#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "criar_tabela.h"
#include "utils.h"
#include "escreve_cabecalho.h"
#include "io_csv.h"
#include "io_registro.h"
#define MAXIMO 500


// Função para comparar dois registros de índice pelo campo idPessoa
static int compararRegistrosIndice(const void *a, const void *b) {
    // Cast dos ponteiros para o tipo correto
    const RegistroIndice *regA = (const RegistroIndice *)a;
    const RegistroIndice *regB = (const RegistroIndice *)b;
    
    // Comparação simples dos campos idPessoa
    if (regA->idPessoa < regB->idPessoa) return -1;
    if (regA->idPessoa > regB->idPessoa) return 1;
    return 0;
}

int criarTabela(const char *nomeArquivoCsv, const char *nomeArquivoDados, const char *nomeArquivoIndice) {
    // Declaração de variáveis
    FILE *arquivoCsv, *arquivoDados, *arquivoIndice;
    CabecalhoPessoa dataHeader;
    char cabecalhoCsv[MAXIMO];
    RegistroIndice *arrayIndice;
    int contadorIndice, capacidadeIndice;
    RegistroPessoa *registro;
    long byteOffsetAtual, tamanhoTotalRegistro;

    // Abrir os arquivos e verificar erros (usando os nomes passados como argumento)
    arquivoCsv = fopen(nomeArquivoCsv, "r");
    arquivoDados = fopen(nomeArquivoDados, "wb");
    arquivoIndice = fopen(nomeArquivoIndice, "wb");

    // Verifica se algum dos arquivos não foi aberto corretamente
    if (arquivoCsv == NULL || arquivoDados == NULL || arquivoIndice == NULL) {
        if (arquivoCsv) fclose(arquivoCsv);
        if (arquivoDados) fclose(arquivoDados);
        if (arquivoIndice) fclose(arquivoIndice);
        return 0;
    }

    // Inicializar e escrever os cabeçalhos dos arquivos binários
    // Cabeçalho do arquivo de dados
    dataHeader.quantidadePessoas = 0;
    dataHeader.quantidadeRemovidos = 0;
    dataHeader.proxByteOffset = 17;
    escreveCabecalhoPessoa(arquivoDados, '0', 0, 0, 17);

    // Cabeçalho do índice
    escreveCabecalhoIndice(arquivoIndice, '0');

    // Pular o cabeçalho do CSV
    fgets(cabecalhoCsv, sizeof(cabecalhoCsv), arquivoCsv);

    // Inicializa variáveis para o array de índices e leitura dos registros
    arrayIndice = NULL;
    contadorIndice = 0;
    capacidadeIndice = 0;
    registro = NULL;
    


    // Cada iteração já lê e processa um registro do CSV
    while ((registro = leRegistroPessoaCsv(arquivoCsv)) != NULL) {
        // Pegar o byte offset atual antes de escrever o registro
        byteOffsetAtual = dataHeader.proxByteOffset;

        // Usa realoc pois foi garantido ram infinita, é melhor que dois i/o de arquivo
        // E mais facil de implementar que uma árvore
        if (contadorIndice == capacidadeIndice) {
            capacidadeIndice = (capacidadeIndice == 0) ? 8 : capacidadeIndice * 2;
            arrayIndice = realloc(arrayIndice, capacidadeIndice * sizeof(RegistroIndice));
        }
        // Inicializa o indice do registro atual e o offset
        arrayIndice[contadorIndice].idPessoa = registro->idPessoa;
        arrayIndice[contadorIndice].byteOffset = byteOffsetAtual;
        contadorIndice++;

        // Calcula o tamanho do registro para o arquivo de dados
        registro->tamanhoRegistro = sizeof(registro->idPessoa) +
                                  sizeof(registro->idadePessoa) +
                                  sizeof(registro->tamanhoNomePessoa) + registro->tamanhoNomePessoa +
                                  sizeof(registro->tamanhoNomeUsuario) + registro->tamanhoNomeUsuario;

        // Tamanho total do registro incluindo os campos fixos para o próximo offset
        tamanhoTotalRegistro = sizeof(registro->removido) + sizeof(registro->tamanhoRegistro) + registro->tamanhoRegistro;

        // Escreve o registro de forma sequencial
        escreveRegistroPessoa(arquivoDados, registro);

        // Atualiza a quantidade de pessoas e o próximo byte offset no cabeçalho
        dataHeader.quantidadePessoas++;
        dataHeader.proxByteOffset += tamanhoTotalRegistro;

        // Libera a memória alocada para o registro lido
        liberaRegistroPessoa(registro);
    }

    // Ordenar o array de índices pelo campo idPessoa
    qsort(arrayIndice, contadorIndice, sizeof(RegistroIndice), compararRegistrosIndice);

    // Escrever os registros de índice ordenados no arquivo de índice
    for (int i = 0; i < contadorIndice; i++) {
        // Escreve o registro de índice
        escreveRegistroIndice(arquivoIndice, &arrayIndice[i]);
    }

    // Atualizar o status dos cabeçalhos para '1' (consistente) e reescrevê-los no início dos arquivos
    // Cabeçalho do arquivo de dados
    fseek(arquivoDados, 0, SEEK_SET);
    escreveCabecalhoPessoa(arquivoDados, '1', dataHeader.quantidadePessoas, dataHeader.quantidadeRemovidos, dataHeader.proxByteOffset);
    // Cabeçalho do arquivo de índice
    fseek(arquivoIndice, 0, SEEK_SET);
    escreveCabecalhoIndice(arquivoIndice, '1');

    // Libera memória e fecha os arquivos
    free(arrayIndice);
    fclose(arquivoCsv);
    fclose(arquivoDados);
    fclose(arquivoIndice);
    // Retorna sucesso
    return 1;
}

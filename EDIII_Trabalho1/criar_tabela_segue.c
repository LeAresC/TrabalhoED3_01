#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "criar_tabela_segue.h"
#include "utils.h"
#define MAXIMO 500

static RegistroSegue* lerRegistroCsv(FILE *arquivoCsv) {
    // Declaração de variáveis
    char linha[256], *campo;
    char *linha_ptr;
    RegistroSegue *registro;

    // Lê uma linha do arquivo CSV, se não houver mais linhas, retorna NULL
    if (fgets(linha, sizeof(linha), arquivoCsv) == NULL) {
        return NULL;
    }

    // Remover o caractere de nova linha, se presente
    linha[strcspn(linha, "\r\n")] = 0;

    // Aloca memória para o registro
    registro = (RegistroSegue*) malloc(sizeof(RegistroSegue));
    if (registro == NULL) {
        return NULL; // Falha na alocação de memória
    }

    // Ler e processar cada campo

    // Usar um ponteiro para permitir que obterProximoCampo atualize a posição
    linha_ptr = linha;

    // Campo idPessoaQueSegue, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo, definir idPessoaQueSegue como -1
        registro->idPessoaQueSegue = -1;
    } else {
        // Campo não nulo
        registro->idPessoaQueSegue = atoi(campo);
    }
    free(campo); // Libera a memória alocada para o campo

    // Campo idPessoaQueESeguida, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo, definir idPessoaQueESeguida como -1
        registro->idPessoaQueESeguida = -1;
    } else {
        // Campo não nulo
        registro->idPessoaQueESeguida = atoi(campo);
    }
    free(campo); // Libera a memória alocada para o campo

    // Campo dataInicioQueSegue, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio
        // Campo fixo, preencher com '$'
        memset(registro->dataInicioQueSegue, '$', sizeof(registro->dataInicioQueSegue));
    } else {
        // Campo não nulo
        // Campo fixo, copia o conteúdo do campo para o registro
        strcpy(registro->dataInicioQueSegue, campo);
    }
    free(campo); // Libera a memória alocada para o campo

    // Campo dataFimQueSegue, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio
        // Campo fixo, preencher com '$'
        memset(registro->dataFimQueSegue, '$', sizeof(registro->dataFimQueSegue));
    } else {
        // Campo não nulo
        // Campo fixo, copia o conteúdo do campo para o registro
        strcpy(registro->dataFimQueSegue, campo);
    }
    free(campo); // Libera a memória alocada para o campo

    // Campo grauAmizade, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio
        // Campo fixo, preencher com '$'
        registro->grauAmizade = '$';
    } else {
        // Campo não nulo
        // Campo fixo, copia o conteúdo do campo para o registro
        registro->grauAmizade = campo[0];
    }
    free(campo); // Libera a memória alocada para o campo

 
    // Acabou de ler todos os campos, inicializar o campo restante
    registro->removido = '1';

    return registro;
}


int criarTabelaSegue(const char *nomeArquivoCsv, const char *nomeArquivoDados) {
    // Declaração de variáveis
    FILE *arquivoCsv, *arquivoDados;
    CabecalhoSegue dataHeader;
    char cabecalhoCsv[MAXIMO];
    RegistroSegue *registro;
    long byteOffsetAtual, tamanhoTotalRegistro;

    // Abrir os arquivos e verificar erros (usando os nomes passados como argumento)
    arquivoCsv = fopen(nomeArquivoCsv, "r");
    arquivoDados = fopen(nomeArquivoDados, "wb");

    // Verifica se algum dos arquivos não foi aberto corretamente
    if (arquivoCsv == NULL || arquivoDados == NULL) {
        if (arquivoCsv) fclose(arquivoCsv);
        if (arquivoDados) fclose(arquivoDados);
        return 0;
    }

    // Inicializar e escrever os cabeçalhos dos arquivos binários
    // Cabeçalho do arquivo de dados
    memset(&dataHeader, 0, sizeof(CabecalhoSegue));
    dataHeader.status = '0';
    dataHeader.quantidadePessoas = 0;
    dataHeader.proxRRN = 9;
    
    // Escreve cada campo do cabeçalho separadamente para evitar problemas de padding
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoDados);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoDados);
    fwrite(&dataHeader.proxRRN, sizeof(dataHeader.proxRRN), 1, arquivoDados);

    // Pular o cabeçalho do CSV
    fgets(cabecalhoCsv, sizeof(cabecalhoCsv), arquivoCsv);

    // Inicializa registro
    registro = NULL;

    // Cada iteração já lê e processa um registro do CSV
    while ((registro = lerRegistroCsv(arquivoCsv)) != NULL) {
        // Pegar o byte offset atual antes de escrever o registro
        byteOffsetAtual = dataHeader.proxRRN;

        // fseek removido, pois a escrita é sequencial no arquivo de dados
        // Assume que não terá problema no ponteiro do arquivo
        // fseek(arquivoDados, byteOffsetAtual, SEEK_SET);

        // Escreve cada campo do registro separadamente para evitar problemas de padding
        fwrite(&registro->removido, sizeof(registro->removido), 1, arquivoDados);
        fwrite(&registro->idPessoaQueSegue, sizeof(registro->idPessoaQueSegue), 1, arquivoDados);
        fwrite(&registro->idPessoaQueESeguida, sizeof(registro->idPessoaQueESeguida), 1, arquivoDados);
        fwrite(&registro->dataInicioQueSegue, sizeof(registro->dataInicioQueSegue), 1, arquivoDados);
        fwrite(&registro->dataFimQueSegue, sizeof(registro->dataFimQueSegue), 1, arquivoDados);
        fwrite(&registro->grauAmizade, sizeof(registro->grauAmizade), 1, arquivoDados);

        // Atualiza a quantidade de pessoas e o próximo byte offset no cabeçalho
        dataHeader.quantidadePessoas++;
        dataHeader.proxRRN += 30; // Tamanho fixo do registro segue

        // Libera a memória alocada para o registro lido
        free(registro);
    }

    // Atualizar o status dos cabeçalhos para '1' (consistente) e reescrevê-los no início dos arquivos
    // Cabeçalho do arquivo de dados
    dataHeader.status = '1';
    fseek(arquivoDados, 0, SEEK_SET);
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoDados);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoDados);
    fwrite(&dataHeader.proxRRN, sizeof(dataHeader.proxRRN), 1, arquivoDados);

    // Libera memória e fecha os arquivos
    fclose(arquivoCsv);
    fclose(arquivoDados);
    // Retorna sucesso
    return 1;
}

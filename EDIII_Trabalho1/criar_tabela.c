#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "criar_tabela.h"


// Função para comparar dois registros de índice pelo campo idPessoa
static int compararRegistrosIndice(const void *a, const void *b) {
    const RegistroIndice *regA = (const RegistroIndice *)a;
    const RegistroIndice *regB = (const RegistroIndice *)b;
    
    if (regA->idPessoa < regB->idPessoa) return -1;
    if (regA->idPessoa > regB->idPessoa) return 1;
    return 0;
}

static char* obterProximoCampo(char **linha) {
    if (**linha == '\0') {
        return NULL;
    }

    // Ponteiros para o início do campo e para percorrer a linha
    char *inicio = *linha;
    char *p = inicio;

    // Avançar até encontrar a vírgula ou o final da string
    while (*p != ',' && *p != '\0') {
        p++;
    }

    // Calcular o tamanho do campo e alocar memória para ele
    int tamanho = p - inicio;
    char *campo = (char*) malloc(tamanho + 1);
    strncpy(campo, inicio, tamanho);
    campo[tamanho] = '\0';

    if (*p == ',') {
        *linha = p + 1; // Mais campos na linha
    } else {
        *linha = p; // Acabou a linha
    }

    return campo;
}

RegistroPessoa* lerRegistroCsv(FILE *arquivoCsv) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), arquivoCsv) == NULL) {
        return NULL;
    }

    // Remover o caractere de nova linha, se presente
    buffer[strcspn(buffer, "\r\n")] = 0;
    
    char *linha = buffer;

    RegistroPessoa *registro = (RegistroPessoa*) malloc(sizeof(RegistroPessoa));
    if (registro == NULL) return NULL;

    char *campo;

    // Ler e processar cada campo

    // Campo idPessoa, garantido que não é nulo
    campo = obterProximoCampo(&linha);
    registro->idPessoa = atoi(campo);
    free(campo);

    // Campo nomePessoa, pode ser nulo
    campo = obterProximoCampo(&linha);
    if (campo == NULL || strlen(campo) == 0) {
        registro->nomePessoa = malloc(1);
        registro->nomePessoa[0] = '\0';
        registro->tamanhoNomePessoa = 0;
    } else {
        registro->nomePessoa = malloc(strlen(campo) + 1);
        strcpy(registro->nomePessoa, campo);
        registro->tamanhoNomePessoa = strlen(registro->nomePessoa);
    }
    free(campo);
    
    // Campo idadePessoa, pode ser nulo
    campo = obterProximoCampo(&linha);
    if (campo == NULL || strlen(campo) == 0) {
        registro->idadePessoa = -1;
    } else {
        registro->idadePessoa = atoi(campo);
    }
    free(campo);

    // Campo nomeUsuario, garantido que não é nulo
    campo = obterProximoCampo(&linha);
    registro->nomeUsuario = malloc(strlen(campo) + 1);
    strcpy(registro->nomeUsuario, campo);
    registro->tamanhoNomeUsuario = strlen(registro->nomeUsuario);
    free(campo);

    // Acabou de ler todos os campos, inicializar os campos restantes
    registro->removido = '0';
    registro->tamanhoRegistro = 0;

    return registro;
}


int criarTabela(char *csvFilename, char *dataFilename, char *indexFilename) {
    // Abrir os arquivos e verificar erros
    FILE *arquivoCsv = fopen(csvFilename, "r");
    FILE *arquivoDados = fopen(dataFilename, "wb");
    FILE *arquivoIndice = fopen(indexFilename, "wb");

    if (arquivoCsv == NULL || arquivoDados == NULL || arquivoIndice == NULL) {
        if (arquivoCsv) fclose(arquivoCsv);
        if (arquivoDados) fclose(arquivoDados);
        if (arquivoIndice) fclose(arquivoIndice);
        return 0;
    }

    // Inicializar e escrever os cabeçalhos dos arquivos binários

    // Cabeçalho do arquivo de dados
    CabecalhoPessoa dataHeader;
    memset(&dataHeader, 0, sizeof(CabecalhoPessoa));
    dataHeader.status = '0';
    dataHeader.proxByteOffset = 17;
    
    // Escreve cada campo do cabeçalho separadamente para evitar problemas de padding
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoDados);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoDados);
    fwrite(&dataHeader.quantidadeRemovidos, sizeof(dataHeader.quantidadeRemovidos), 1, arquivoDados);
    fwrite(&dataHeader.proxByteOffset, sizeof(dataHeader.proxByteOffset), 1, arquivoDados);

    // Cabeçalho do índice
    CabecalhoIndice indexHeader;
    memset(&indexHeader, 0, sizeof(CabecalhoIndice));
    indexHeader.status = '0';
    memset(indexHeader.lixo, '$', 11);

    // Escreve cada campo do cabeçalho separadamente para evitar problemas de padding
    fwrite(&indexHeader.status, sizeof(indexHeader.status), 1, arquivoIndice);
    fwrite(&indexHeader.lixo, sizeof(indexHeader.lixo), 1, arquivoIndice);

    // Pular o cabeçalho do CSV
    char cabecalhoCsv[256];
    fgets(cabecalhoCsv, sizeof(cabecalhoCsv), arquivoCsv);


    RegistroIndice *arrayIndice = NULL;
    int contadorIndice = 0;
    int capacidadeIndice = 0;
    RegistroPessoa *registro;

    // Cada iteração já lê e processa um registro do CSV
    while ((registro = lerRegistroCsv(arquivoCsv)) != NULL) {
        // Pegar o byte offset atual antes de escrever o registro
        long byteOffsetAtual = dataHeader.proxByteOffset;

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
        long tamanhoTotalRegistro = sizeof(registro->removido) + sizeof(registro->tamanhoRegistro) + registro->tamanhoRegistro;

        // Escrever o registro no arquivo de dados no offset correto
        // Usa fseek para garantir que está escrevendo no lugar certo
        fseek(arquivoDados, byteOffsetAtual, SEEK_SET);

        // Escreve cada campo do registro separadamente para evitar problemas de padding
        fwrite(&registro->removido, sizeof(registro->removido), 1, arquivoDados);
        fwrite(&registro->tamanhoRegistro, sizeof(registro->tamanhoRegistro), 1, arquivoDados);
        fwrite(&registro->idPessoa, sizeof(registro->idPessoa), 1, arquivoDados);
        fwrite(&registro->idadePessoa, sizeof(registro->idadePessoa), 1, arquivoDados);
        fwrite(&registro->tamanhoNomePessoa, sizeof(registro->tamanhoNomePessoa), 1, arquivoDados);
        // Escreve o nomePessoa se não for nulo
        if (registro->tamanhoNomePessoa > 0) {
            fwrite(registro->nomePessoa, registro->tamanhoNomePessoa, 1, arquivoDados);
        }
        fwrite(&registro->tamanhoNomeUsuario, sizeof(registro->tamanhoNomeUsuario), 1, arquivoDados);
        fwrite(registro->nomeUsuario, registro->tamanhoNomeUsuario, 1, arquivoDados);

        // Atualiza a quantidade de pessoas e o próximo byte offset no cabeçalho
        dataHeader.quantidadePessoas++;
        dataHeader.proxByteOffset += tamanhoTotalRegistro;

        free(registro->nomePessoa);
        free(registro->nomeUsuario);
        free(registro);
    }

    // Ordenar o array de índices pelo campo idPessoa
    qsort(arrayIndice, contadorIndice, sizeof(RegistroIndice), compararRegistrosIndice);

    // Escrever os registros de índice ordenados no arquivo de índice
    for (int i = 0; i < contadorIndice; i++) {
        fwrite(&arrayIndice[i].idPessoa, sizeof(arrayIndice[i].idPessoa), 1, arquivoIndice);
        fwrite(&arrayIndice[i].byteOffset, sizeof(arrayIndice[i].byteOffset), 1, arquivoIndice);
    }

    // Atualizar o status dos cabeçalhos para '1' (consistente) e reescrevê-los no início dos arquivos
    dataHeader.status = '1';
    fseek(arquivoDados, 0, SEEK_SET);
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoDados);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoDados);
    fwrite(&dataHeader.quantidadeRemovidos, sizeof(dataHeader.quantidadeRemovidos), 1, arquivoDados);
    fwrite(&dataHeader.proxByteOffset, sizeof(dataHeader.proxByteOffset), 1, arquivoDados);
    indexHeader.status = '1';
    fseek(arquivoIndice, 0, SEEK_SET);
    fwrite(&indexHeader.status, sizeof(indexHeader.status), 1, arquivoIndice);
    fwrite(&indexHeader.lixo, sizeof(indexHeader.lixo), 1, arquivoIndice);

    free(arrayIndice);
    fclose(arquivoCsv);
    fclose(arquivoDados);
    fclose(arquivoIndice);

    return 1;
}

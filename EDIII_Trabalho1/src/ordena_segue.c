#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "ordena_segue.h"
#include "utils.h"
#include "io_registro.h"
#include "io_cabecalho.h"
#define MAXIMO 500

static int compararNulos(int a_null, int b_null) {
    if (a_null && !b_null) return 1; // a é nulo, b não é nulo
    if (!a_null && b_null) return -1; // a não é nulo, b é nulo
    return 0; // ambos são nulos ou ambos não são nulos
}

static int comparaInt(int a, int b) {
    int a_null = (a == -1); // considerando -1 como valor nulo
    int b_null = (b == -1); // considerando -1 como valor nulo
    // Comparar nulos primeiro
    int nuloComp = compararNulos(a_null, b_null);
    // Se ambos não são nulos, comparar os valores
    if (nuloComp != 0) return nuloComp;
    if (a < b) return -1; // a é menor que b
    if (a > b) return 1;  // a é maior que b
    return 0; // são iguais
}

static int comparaStr(const char *a, const char *b) {
    int a_null = (a[0] == '$'); // considerando string começando com '$' como nula
    int b_null = (b[0] == '$'); // considerando string começando com '$' como nula
    // Comparar nulos primeiro
    int nuloComp = compararNulos(a_null, b_null);
    // Se ambos não são nulos, comparar os valores
    if (nuloComp != 0) return nuloComp;
    return strcmp(a, b); // comparar strings normalmente
}

static int compararRegistrosSegue(const void *a, const void *b) {
    // Cast dos ponteiros para o tipo correto
    const RegistroSegue *regA = (const RegistroSegue *)a;
    const RegistroSegue *regB = (const RegistroSegue *)b;
    int resultado;

    // Comparar por idPessoaQueSegue primeiro
    resultado = comparaInt(regA->idPessoaQueSegue, regB->idPessoaQueSegue);
    if (resultado != 0) return resultado;
    // Se idPessoaQueSegue for igual, comparar por idPessoaQueESeguida
    resultado = comparaInt(regA->idPessoaQueESeguida, regB->idPessoaQueESeguida);
    if (resultado != 0) return resultado;
    // Se idPessoaQueESeguida também for igual, comparar por dataInicioQueSegue
    resultado = comparaStr(regA->dataInicioQueSegue, regB->dataInicioQueSegue);
    if (resultado != 0) return resultado;
    // Se dataInicioQueSegue também for igual, comparar por dataFimQueSegue
    resultado = comparaStr(regA->dataFimQueSegue, regB->dataFimQueSegue);
    return resultado;
}

int ordenaSegue(const char *nomeArquivoDesordenado, const char *nomeArquivoOrdenado) {
    // Declaração de variáveis
    FILE *arquivoDesordenado, *arquivoOrdenado;
    CabecalhoSegue dataHeader;
    RegistroSegue *registro;

    // Abrir os arquivos e verificar erros (usando os nomes passados como argumento)
    arquivoDesordenado = fopen(nomeArquivoDesordenado, "rb");
    arquivoOrdenado = fopen(nomeArquivoOrdenado, "wb");

    // Verifica se algum dos arquivos não foi aberto corretamente
    if (arquivoDesordenado == NULL || arquivoOrdenado == NULL) {
        if (arquivoDesordenado) fclose(arquivoDesordenado);
        if (arquivoOrdenado) fclose(arquivoOrdenado);
        return 0;
    }

    // Ler o cabeçalho do arquivo desordenado
    dataHeader = leCabecalhoSegue(arquivoDesordenado);

    // Escrever o cabeçalho no arquivo ordenado
    escreveCabecalhoSegue(arquivoOrdenado, '0', dataHeader.quantidadePessoas, dataHeader.proxRRN);

    registro = (RegistroSegue*) malloc(sizeof(RegistroSegue) * dataHeader.quantidadePessoas);

    for (int i = 0; i < dataHeader.quantidadePessoas; i++) {
        // Lê cada registro do arquivo desordenado
        leRegistroSegue(arquivoDesordenado, &registro[i]);
    }
    
    // Fechar o arquivo desordenado
    fclose(arquivoDesordenado);

    // Ordenar os registros baseado no comparador
    qsort(registro, dataHeader.quantidadePessoas, sizeof(RegistroSegue), compararRegistrosSegue);

    // Reescrever o cabeçalho no arquivo ordenado
    fseek(arquivoOrdenado, 0, SEEK_SET);
    escreveCabecalhoSegue(arquivoOrdenado, '1', dataHeader.quantidadePessoas, dataHeader.proxRRN);

    for (int i = 0; i < dataHeader.quantidadePessoas; i++) {
        // Escreve cada registro ordenado no arquivo ordenado
        escreveRegistroSegue(arquivoOrdenado, &registro[i]);
    }

    // Fechar o arquivo ordenado
    fclose(arquivoOrdenado);
    free(registro);
    return 1;
}
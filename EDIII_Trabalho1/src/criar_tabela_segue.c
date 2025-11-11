#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "criar_tabela_segue.h"
#include "utils.h"
#include "io_cabecalho.h"
#include "io_csv.h"
#include "io_registro.h"
#define MAXIMO 500

int criarTabelaSegue(const char *nomeArquivoCsv, const char *nomeArquivoDados) {
    // Declaração de variáveis
    FILE *arquivoCsv, *arquivoDados;
    CabecalhoSegue dataHeader;
    char cabecalhoCsv[MAXIMO];
    RegistroSegue *registro;

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
    dataHeader.quantidadePessoas = 0;
    dataHeader.proxRRN = 9;
    escreveCabecalhoSegue(arquivoDados, '0', dataHeader.quantidadePessoas, dataHeader.proxRRN);

    // Pular o cabeçalho do CSV
    fgets(cabecalhoCsv, sizeof(cabecalhoCsv), arquivoCsv);

    // Inicializa registro
    registro = NULL;
    
    // Cada iteração já lê e processa um registro do CSV
    while ((registro = leRegistroSegueCsv(arquivoCsv)) != NULL) {
        // Escreve o registro de forma sequencial
        escreveRegistroSegue(arquivoDados, registro);

        // Atualiza a quantidade de pessoas e o próximo byte offset no cabeçalho
        dataHeader.quantidadePessoas++;
        dataHeader.proxRRN += 30; // Tamanho fixo do registro segue

        // Libera a memória alocada para o registro lido
        free(registro);
    }

    // Atualizar o status dos cabeçalhos para '1' (consistente) e reescrevê-los no início dos arquivos
    // Cabeçalho do arquivo de dados
    fseek(arquivoDados, 0, SEEK_SET);
    escreveCabecalhoSegue(arquivoDados, '1', dataHeader.quantidadePessoas, dataHeader.proxRRN);

    // Libera memória e fecha os arquivos
    fclose(arquivoCsv);
    fclose(arquivoDados);
    // Retorna sucesso
    return 1;
}

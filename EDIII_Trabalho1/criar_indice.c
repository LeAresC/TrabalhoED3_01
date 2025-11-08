#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "criar_indice.h"
#include "escreve_cabecalho.h"

int criarIndice(const char *nomeArquivo){
    // Declaração de variáveis
    FILE *arquivoIndice;
    CabecalhoIndice dataHeader;
    // Abre o arquivo de índice para escrita binária
    arquivoIndice = fopen(nomeArquivo, "wb");
    // Verifica se o arquivo foi aberto corretamente
    if(arquivoIndice == NULL){
        return 0;
    }

    // Chama a função para escrever o cabeçalho do índice com status '1' (consistente)
    escreveCabecalhoIndice(arquivoIndice, '1');

    // Fecha o arquivo de índice e retorna sucesso
    fclose(arquivoIndice);
    return 1;
}

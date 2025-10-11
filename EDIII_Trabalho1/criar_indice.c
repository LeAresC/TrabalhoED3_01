#include <stdio.h>
#include <string.h>
#include "criar_indice.h"

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

    // Inicializa o cabeçalho do índice
    dataHeader.status = '1';
    memset(dataHeader.lixo, '$', sizeof(dataHeader.lixo));

    // Escreve o cabeçalho no arquivo de índice
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoIndice);
    fwrite(dataHeader.lixo, sizeof(dataHeader.lixo), 1, arquivoIndice);

    // Fecha o arquivo de índice e retorna sucesso
    fclose(arquivoIndice);
    return 1;
}

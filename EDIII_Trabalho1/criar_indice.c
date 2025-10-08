#include <stdio.h>
#include <string.h>
#include "criar_indice.h"

int criarIndice(char *nomeArquivo){
    FILE *arquivoIndice;
    CabecalhoIndice dataHeader;
    
    arquivoIndice = fopen(nomeArquivo, "wb");
    if(arquivoIndice == NULL){
        return 0;
    }

    // Inicializa o cabeçalho do índice
    dataHeader.status = '1';
    memset(dataHeader.lixo, '$', sizeof(dataHeader.lixo));

    // Escreve o cabeçalho no arquivo de índice
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoIndice);
    fwrite(dataHeader.lixo, sizeof(dataHeader.lixo), 1, arquivoIndice);

    fclose(arquivoIndice);
    return 1;
}

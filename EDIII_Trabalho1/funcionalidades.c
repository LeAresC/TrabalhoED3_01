#include <stdio.h>
#include <string.h>
#include "funcionalidades.h"

int gera_indice_primario(char *nome_do_arquivo){
    FILE *arq_de_indice;
    arq_de_indice = fopen(nome_do_arquivo, "wb");

    if(arq_de_indice == NULL){
        return 0;
    }
    char lixo[] = "$$$$$$$$$$$";
    char consistencia[] = "1";

    fwrite(consistencia, sizeof(char), strlen(consistencia), arq_de_indice);
    fwrite(lixo, sizeof(char), strlen(lixo), arq_de_indice);

    fclose(arq_de_indice);
    return 1;
}

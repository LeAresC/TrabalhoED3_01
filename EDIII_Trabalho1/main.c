#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcionalidades.h"

#define MAXIMO 500

int main (int argc, char *argv[]){

    int funcionalidade = atoi(argv[1]);

    if(funcionalidade == 1){
        char *nome_do_arquivo = (char *) malloc(MAXIMO*sizeof(char));
        strcpy(nome_do_arquivo, argv[2]);
        int ret = gera_indice_primario(nome_do_arquivo);
        if(ret == 1)
        printf("Arquivo Criado com sucesso\n");
        else printf("Falha na criacao do Arquivo\n");
    }

    if(funcionalidade == 2){

    }

    if(funcionalidade == 3){

    }
    if(funcionalidade == 4){

    }
}

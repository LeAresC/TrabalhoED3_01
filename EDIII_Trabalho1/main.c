#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcionalidades.h"
#include "create_table.h"


#define MAXIMO 500

void binarioNaTela(char *nomeArquivoBinario);

int main () {

    int funcionalidade;
    scanf("%d", &funcionalidade);

//    if(funcionalidade == 1){
//        char *nome_do_arquivo = (char *) malloc(MAXIMO*sizeof(char));
//        strcpy(nome_do_arquivo, argv[2]);
//        int ret = gera_indice_primario(nome_do_arquivo);
//        if(ret == 1)
//        printf("Arquivo Criado com sucesso\n");
//        else printf("Falha na criacao do Arquivo\n");
//    }

    if(funcionalidade == 2){
        char csvFilename[100], dataFilename[100], indexFilename[100];
        scanf("%s %s %s", csvFilename, dataFilename, indexFilename);

        createTable(csvFilename, dataFilename, indexFilename);

        binarioNaTela(dataFilename);
        binarioNaTela(indexFilename);

    }

    if(funcionalidade == 3){

    }
    if(funcionalidade == 4){

    }
}

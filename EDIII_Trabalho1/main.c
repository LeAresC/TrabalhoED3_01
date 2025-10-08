#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "criar_indice.h"
#include "criar_tabela.h"

#define MAXIMO 500

void binarioNaTela(char *nomeArquivoBinario);

int main () {
    int funcionalidade;
    int resultado;
    scanf("%d", &funcionalidade);

    if(funcionalidade == 1){
        // Lê o nome do arquivo de índice
        char nomeIndex[MAXIMO];
        scanf("%s", nomeIndex);
        // Chama a função para criar o índice e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarIndice(nomeIndex);
        if (resultado == 1) {
            binarioNaTela(nomeIndex);
        } else {
            printf("Falha no processamento do arquivo.\n");
        }
    }

    if (funcionalidade == 2) {
        // Lê os nomes dos arquivos
        char nomeCsv[MAXIMO], nomeData[MAXIMO], nomeIndex[MAXIMO];
        scanf("%s %s %s", nomeCsv, nomeData, nomeIndex);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarTabela(nomeCsv, nomeData, nomeIndex);
        if (resultado == 1) {
            binarioNaTela(nomeData);
            binarioNaTela(nomeIndex);
        } else {
            printf("Falha no processamento do arquivo.\n");
        }

    }

    if(funcionalidade == 3){

    }
    if(funcionalidade == 4){

    }
}

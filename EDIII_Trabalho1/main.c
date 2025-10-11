#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "criar_indice.h"
#include "criar_tabela.h"

void binarioNaTela(char *nomeArquivoBinario);

int main () {
    // Declaração de variáveis
    int funcionalidade;
    int resultado;

    // Lê o número da funcionalidade
    scanf("%d", &funcionalidade);

    if(funcionalidade == 1){
        // Ler o nome do arquivo de índice e chamar a função
        char nomeIndex[MAXIMO];
        scanf("%s", nomeIndex);
        resultado = criarIndice(nomeIndex);
        if (resultado == 1) {
            binarioNaTela(nomeIndex);
        } else {
            printf("Falha no processamento do arquivo.\n");
        }
    }

    if (funcionalidade == 2) {
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char nome_csv[MAXIMO], nome_dados[MAXIMO], nome_indice[MAXIMO];
        scanf("%s %s %s", nome_csv, nome_dados, nome_indice);
        resultado = criarTabela(nome_csv, nome_dados, nome_indice);
        if (resultado == 1) {
            binarioNaTela(nome_dados);
            binarioNaTela(nome_indice);
        } else {
            printf("Falha no processamento do arquivo.\n");
        }

    }

    if(funcionalidade == 3){

    }
    if(funcionalidade == 4){

    }
}

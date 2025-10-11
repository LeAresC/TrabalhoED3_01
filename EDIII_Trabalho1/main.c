#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "criar_indice.h"
#include "criar_tabela.h"
#include "auxiliares_busca.h"
#define MAXIMO 500

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
        // Chama a função para criar o índice e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarIndice(nomeIndex);
        if (resultado == 1) {
            // Imprime o arquivo binário na tela
            binarioNaTela(nomeIndex);
        } else {
            // Falha na criação do índice, imprime como definido
            printf("Falha no processamento do arquivo.\n");
        }
    }

    if (funcionalidade == 2) {
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char nome_csv[MAXIMO], nome_dados[MAXIMO], nome_indice[MAXIMO];
        scanf("%s %s %s", nome_csv, nome_dados, nome_indice);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarTabela(nome_csv, nome_dados, nome_indice);
        if (resultado == 1) {
            // Imprime os arquivos binários na tela
            binarioNaTela(nome_dados);
            binarioNaTela(nome_indice);
        } else {
            // Falha na criação da tabela, imprime como definido
            printf("Falha no processamento do arquivo.\n");
        }

    }

    if(funcionalidade == 3){
        //Lê o nome do arquivo binário
        char arquivo_bin[MAXIMO];
        scanf("%s", arquivo_bin);

        //Returna 2 se tudo certo, 1 se o não houver registro e 0 se houver erro na abertura do arquivo
        resultado = lerArquivoBinario(arquivo_bin);

        if(resultado == 0)
        {
            erroAbertura();
        }
        if(resultado == 1)
        {
            erroRegistro();  
        }

    }
    if (funcionalidade == 4)
    {
        char arquivo_dados[MAXIMO];
        char arquivo_indice[MAXIMO];
        int qtd;
        scanf("%s", arquivo_dados);
        scanf("%s", arquivo_indice);
        scanf("%d", &qtd);

        resultado = buscaEmArquivo(arquivo_dados, arquivo_indice, qtd);
    
        if(resultado == 0)
        {
            erroAbertura();
        }
    }
}

//Autores : Luis Eduardo Aires Coimbra, Número USP: 15472565; Pedro Teodoro do Nascimento, Número USP: 15579459

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
            erroAbertura();
        }
    }

    if (funcionalidade == 2) {
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char nomeCsv[MAXIMO], nomeDados[MAXIMO], nomeIndice[MAXIMO];
        scanf("%s %s %s", nomeCsv, nomeDados, nomeIndice);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarTabela(nomeCsv, nomeDados, nomeIndice);
        if (resultado == 1) {
            // Imprime os arquivos binários na tela
            binarioNaTela(nomeDados);
            binarioNaTela(nomeIndice);
        } else {
            // Falha na criação da tabela, imprime como definido
            erroAbertura();
        }

    }

    if(funcionalidade == 3){
        //Lê o nome do arquivo binário
        char arquivoBin[MAXIMO];
        scanf("%s", arquivoBin);

        //Returna 2 se tudo certo, 1 se o não houver registro e 0 se houver erro na abertura do arquivo
        resultado = lerArquivoBinario(arquivoBin);

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
        // Le os dados
        char arquivoDados[MAXIMO];
        char arquivoIndice[MAXIMO];
        int qtd;
        scanf("%s", arquivoDados);
        scanf("%s", arquivoIndice);
        scanf("%d", &qtd);

        // Retorna 1 se tudo certo e 0 se houver erro na abertura
        resultado = buscaEmArquivo(arquivoDados, arquivoIndice, qtd);
    
        if(resultado == 0)
        {
            erroAbertura();
        }
    }
}

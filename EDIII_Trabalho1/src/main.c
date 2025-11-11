// Autores : Luis Eduardo Aires Coimbra, Número USP: 15472565; Pedro Teodoro do Nascimento, Número USP: 15579459

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "criar_indice.h"
#include "criar_tabela.h"
#include "auxiliares_busca.h"
#include "funcionalidade_3.h"
#include "funcionalidade_4.h"
#include "funcionalidade_5.h"
#include "funcionalidade_6.h"
#include "funcionalidade_7.h"
#include "utilidades.h"
#include "ordena_segue.h"
#include "criar_tabela_segue.h"
#include "junta_pessoa_segue.h"
#define MAXIMO 500

int main()
{
    // Declaração de variáveis
    int funcionalidade;
    int resultado;

    // Lê o número da funcionalidade
    scanf("%d", &funcionalidade);

    if (funcionalidade == 1)
    {
        // Ler o nome do arquivo de índice e chamar a função
        char nomeIndex[MAXIMO];
        scanf("%s", nomeIndex);
        // Chama a função para criar o índice e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarIndice(nomeIndex);
        if (resultado == 1)
        {
            // Imprime o arquivo binário na tela
            binarioNaTela(nomeIndex);
        }
        else
        {
            // Falha na criação do índice, imprime como definido
            erroAbertura();
        }
    }

    if (funcionalidade == 2)
    {
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char nomeCsv[MAXIMO], nomeDados[MAXIMO], nomeIndice[MAXIMO];
        scanf("%s %s %s", nomeCsv, nomeDados, nomeIndice);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarTabela(nomeCsv, nomeDados, nomeIndice);
        if (resultado == 1)
        {
            // Imprime os arquivos binários na tela
            binarioNaTela(nomeDados);
            binarioNaTela(nomeIndice);
        }
        else
        {
            // Falha na criação da tabela, imprime como definido
            erroAbertura();
        }
    }

    if (funcionalidade == 3)
    {
        // Lê o nome do arquivo binário
        char arquivoBin[MAXIMO];
        scanf("%s", arquivoBin);

        // Returna 2 se tudo certo, 1 se o não houver registro e 0 se houver erro na abertura do arquivo
        resultado = lerArquivoBinario(arquivoBin);

        if (resultado == 0)
        {
            erroAbertura();
        }
        if (resultado == 1)
        {
            erroRegistro();
        }
    }
    if (funcionalidade == 4)
    {
        // Le os dados
        char arquivoDados[MAXIMO], arquivoIndice[MAXIMO];
        int N;
        scanf("%s %s %d", arquivoDados, arquivoIndice, &N);
        resultado = buscaNDados(arquivoDados, arquivoIndice, N);
        if (resultado == 0)
        {
            erroAbertura();
        }
    }
    if (funcionalidade == 5)
    {
        char arquivoDados[MAXIMO], arquivoIndice[MAXIMO];
        int N;
        scanf("%s %s %d", arquivoDados, arquivoIndice, &N);
        resultado = removeArquivoPessoa(arquivoDados, arquivoIndice, N);
        if (resultado == 0)
        {
            erroAbertura();
        }
        else
        {
            binarioNaTela(arquivoDados);
            binarioNaTela(arquivoIndice);
        }
    }
    if (funcionalidade == 6)
    {
        char arquivoDados[MAXIMO], arquivoIndice[MAXIMO];
        int N;
        scanf("%s %s %d", arquivoDados, arquivoIndice, &N);
        resultado = insereNoFinal(arquivoDados, arquivoIndice, N);
        if (resultado == 0)
        {
            erroAbertura();
        }
        else
        {
            binarioNaTela(arquivoDados);
            binarioNaTela(arquivoIndice);
        }
    }
    if (funcionalidade == 7)
    {
        char arquivoDados[MAXIMO], arquivoIndice[MAXIMO];
        int N;
        scanf("%s %s %d", arquivoDados, arquivoIndice, &N);
        resultado = atualizaArquivos(arquivoDados, arquivoIndice, N);
        if (resultado == 0)
        {
            erroAbertura();
        }
        else
        {
            binarioNaTela(arquivoDados);
            binarioNaTela(arquivoIndice);
        }
    }
    if (funcionalidade == 8)
    {
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char nomeCsv[MAXIMO], nomeDados[MAXIMO];
        scanf("%s %s", nomeCsv, nomeDados);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = criarTabelaSegue(nomeCsv, nomeDados);
        if (resultado == 1)
        {
            // Imprime os arquivos binários na tela
            binarioNaTela(nomeDados);
        }
        else
        {
            // Falha na criação da tabela, imprime como definido
            erroAbertura();
        }
    }

    if (funcionalidade == 9)
    {
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char desordenado[MAXIMO], ordenado[MAXIMO];
        scanf("%s %s", desordenado, ordenado);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = ordenaSegue(desordenado, ordenado);
        if (resultado == 1)
        {
            // Imprime os arquivos binários na tela
            binarioNaTela(ordenado);
        }
        else
        {
            // Falha na criação da tabela, imprime como definido
            erroAbertura();
        }
    }

    if (funcionalidade == 10)
    {
        // Ler os nomes dos arquivos e chamar a função criarTabela
        char ordenado[MAXIMO], pessoa[MAXIMO], indice[MAXIMO];
        int quantidade;
        scanf("%s %s %s %d", pessoa, ordenado, indice, &quantidade);
        // Chama a função para criar a tabela e somente imprime o binário na tela se der certo
        // Retorna 1 se tudo der certo, 0 caso contrário
        resultado = juntaPessoaSegue(pessoa, ordenado, indice, quantidade);
        if (resultado == 0)
        {
            // Falha na criação da tabela, imprime como definido
            erroAbertura();
        }
    }
}

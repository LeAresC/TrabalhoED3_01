#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#define MAXIMO 2000

void imprimeListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    for(int i = 0; i < qtdPessoas; i++){
        int qtdSegue = ListaAdjacencia[i].tamanho;
        No *at = ListaAdjacencia[i].inicio;
        for(int j = 0; j < qtdSegue; j++){
            printf("%s, " ,at->nomeUsuarioQueSegue);
            printf("%s, " ,at->nomeUsuarioQueESeguida);
            if(strcmp(at->dataInicioQueSegue, "-") != 0)
            printf("%s, " ,at->dataInicioQueSegue);
            else printf("NULO, ");
            if(strcmp(at->dataFimQueSegue, "-") != 0)
            printf("%s, " ,at->dataFimQueSegue);
            else printf("NULO, ");
            if(at->grauAmizade != '-')
            printf("%c" ,at->grauAmizade);
            else printf("NULO");
            printf("\n");
            at = at->prox;
        }
        printf("\n");
    }
}

void InsereAdjacencia(FILE *arqP, FILE *arqS, int Map[], Lista* ListaDeAdjacencia, int qtdSegues, int qtdPessoas, RegistroIndice **DadosIndice)
{
    fseek(arqS, SEEK_SET, 9);
    int Contador = 0;
    while (Contador < qtdSegues)
    {
        RegistroSegue RegAtualS;
        leRegistroSegue(arqS, &RegAtualS);
        if(RegAtualS.removido == '1') 
        {
            fseek(arqS, SEEK_CUR, 29);
            continue;
        }


        int Indice = Map[RegAtualS.idPessoaQueSegue];
        int TamanhoAtual = ListaDeAdjacencia[Indice].tamanho;
        No* NoAtual = (No *)malloc(sizeof(No));
        if(TamanhoAtual == 0)
        {
            ListaDeAdjacencia[Indice].inicio = NoAtual;
        }
        else
        {
            ListaDeAdjacencia[Indice].fim->prox = NoAtual;
        }
        ListaDeAdjacencia[Indice].fim = NoAtual;

        long Offset = buscaBinariaIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueSegue);
        fseek(arqP, SEEK_SET, Offset);
        RegistroPessoa *RegAtualP = leRegistroPessoa(arqP);
        NoAtual->nomeUsuarioQueSegue = malloc(MAXIMO);
        strcpy(NoAtual->nomeUsuarioQueSegue,RegAtualP->nomePessoa);

        Offset = buscaBinariaIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueESeguida);
        fseek(arqP, SEEK_SET, Offset);
        RegAtualP = leRegistroPessoa(arqP); 
        NoAtual->nomeUsuarioQueESeguida = malloc(MAXIMO);
        strcpy(NoAtual->nomeUsuarioQueESeguida, RegAtualP->nomePessoa);

        strcpy(NoAtual->dataInicioQueSegue,RegAtualS.dataInicioQueSegue);

        strcpy(NoAtual->dataFimQueSegue, RegAtualS.dataFimQueSegue);

        NoAtual->grauAmizade = RegAtualS.grauAmizade;
        NoAtual->prox == NULL;

        ListaDeAdjacencia[Indice].tamanho++;
        Contador++;
    }
}

Lista* criaListaAdjacencia(FILE *arqP, FILE *arqS, RegistroIndice **DadosIndice, int qtdPessoas, int qtdSegues)
{
   Lista* ListaDeAdjacencia = (Lista *) malloc(sizeof(Lista) * qtdPessoas);

    int Map[MAXIMO];
    memset(Map, -1, sizeof(Map));

    for (int i = 0; i < qtdPessoas; i++)
    {
        Map[DadosIndice[i]->idPessoa] = i;
        ListaDeAdjacencia[i].tamanho = 0;
    }
    InsereAdjacencia(arqP, arqS, Map, ListaDeAdjacencia, qtdSegues, qtdPessoas, DadosIndice);
    return ListaDeAdjacencia;
}
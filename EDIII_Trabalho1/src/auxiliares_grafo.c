#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#define MAXIMO 2000

int TipoGrafo = 0;

void imprimeListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    for (int i = 0; i < qtdPessoas; i++)
    {
        int qtdSegue = ListaAdjacencia[i].tamanho;
        No *at = ListaAdjacencia[i].inicio;
        for (int j = 0; j < qtdSegue; j++)
        {
            if (TipoGrafo == 0)
            {
                printf("%s, ", at->nomeUsuarioQueSegue);
                printf("%s, ", at->nomeUsuarioQueESeguida);
            }
            else
            {
                printf("%s, ", at->nomeUsuarioQueESeguida);
                printf("%s, ", at->nomeUsuarioQueSegue);
            }
            if (strcmp(at->dataInicioQueSegue, "-") != 0)
                printf("%s, ", at->dataInicioQueSegue);
            else
                printf("NULO, ");
            if (strcmp(at->dataFimQueSegue, "-") != 0)
                printf("%s, ", at->dataFimQueSegue);
            else
                printf("NULO, ");
            if (at->grauAmizade != '-')
                printf("%c", at->grauAmizade);
            else
                printf("NULO");
            printf("\n");
            at = at->prox;
        }
        printf("\n");
    }
}

void InsereAdjacencia(FILE *arqP, FILE *arqS, int Map[], Lista *ListaDeAdjacencia, int qtdSegues, int qtdPessoas, RegistroIndice **DadosIndice)
{
    fseek(arqS, SEEK_SET, 9);
    int Contador = 0;
    while (Contador < qtdSegues)
    {
        RegistroSegue RegAtualS;
        leRegistroSegue(arqS, &RegAtualS);
        if (RegAtualS.removido == '1')
        {
            fseek(arqS, SEEK_CUR, 29);
            continue;
        }

        int Indice = Map[RegAtualS.idPessoaQueSegue];
        int TamanhoAtual = ListaDeAdjacencia[Indice].tamanho;
        No *NoAtual = (No *)malloc(sizeof(No));
        if (TamanhoAtual == 0)
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
        strcpy(NoAtual->nomeUsuarioQueSegue, RegAtualP->nomePessoa);

        Offset = buscaBinariaIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueESeguida);
        fseek(arqP, SEEK_SET, Offset);
        RegAtualP = leRegistroPessoa(arqP);
        NoAtual->nomeUsuarioQueESeguida = malloc(MAXIMO);
        strcpy(NoAtual->nomeUsuarioQueESeguida, RegAtualP->nomePessoa);

        strcpy(NoAtual->dataInicioQueSegue, RegAtualS.dataInicioQueSegue);

        strcpy(NoAtual->dataFimQueSegue, RegAtualS.dataFimQueSegue);

        NoAtual->grauAmizade = RegAtualS.grauAmizade;
        NoAtual->prox = NULL;

        ListaDeAdjacencia[Indice].tamanho++;
        Contador++;
    }
}

// Retorna < 0 se d1 < d2, 0 se iguais, > 0 se d1 > d2
int compararDatas(char *d1, char *d2)
{
    // Comparar Ano
    int res = strncmp(&d1[6], &d2[6], 4);
    if (res != 0)
        return res;

    // Comparar Mês
    res = strncmp(&d1[3], &d2[3], 2);
    if (res != 0)
        return res;

    // Comparar Dia
    return strncmp(d1, d2, 2);
}

void trocaConteudo(No *no1, No *no2)
{
    No aux;
    aux.nomeUsuarioQueESeguida = malloc(sizeof(MAXIMO));
    aux.nomeUsuarioQueSegue = malloc(sizeof(MAXIMO));

    strcpy(aux.dataInicioQueSegue, no1->dataInicioQueSegue);
    strcpy(aux.dataFimQueSegue, no1->dataFimQueSegue);
    aux.grauAmizade = no1->grauAmizade;
    strcpy(aux.nomeUsuarioQueESeguida, no1->nomeUsuarioQueESeguida);
    strcpy(aux.nomeUsuarioQueSegue, no1->nomeUsuarioQueSegue);

    strcpy(no1->dataInicioQueSegue, no2->dataInicioQueSegue);
    strcpy(no1->dataFimQueSegue, no2->dataFimQueSegue);
    no1->grauAmizade = no2->grauAmizade;
    strcpy(no1->nomeUsuarioQueESeguida, no2->nomeUsuarioQueESeguida);
    strcpy(no1->nomeUsuarioQueSegue, no2->nomeUsuarioQueSegue);

    strcpy(no2->dataInicioQueSegue, aux.dataInicioQueSegue);
    strcpy(no2->dataFimQueSegue, aux.dataFimQueSegue);
    no2->grauAmizade = aux.grauAmizade;
    strcpy(no2->nomeUsuarioQueESeguida, aux.nomeUsuarioQueESeguida);
    strcpy(no2->nomeUsuarioQueSegue, aux.nomeUsuarioQueSegue);

    free(aux.nomeUsuarioQueESeguida);
    free(aux.nomeUsuarioQueSegue);
}

void ordenarListaInterna(Lista ListaAtual)
{
    if (ListaAtual.tamanho == 0 || ListaAtual.inicio == NULL)
    {
        return;
    }
    int N = ListaAtual.tamanho;
    for (int i = 0; i < N - 1; i++)
    {
        No *at = ListaAtual.inicio;
        for (int j = 0; j < N - i - 1; j++)
        {
            int comparador;
            if (TipoGrafo == 0)
                comparador = strcmp(at->nomeUsuarioQueESeguida, at->prox->nomeUsuarioQueESeguida);
            else
                comparador = strcmp(at->nomeUsuarioQueSegue, at->prox->nomeUsuarioQueSegue);
            if (comparador == 0)
            {
                comparador = compararDatas(at->dataInicioQueSegue, at->prox->dataInicioQueSegue);
            }
            if (comparador > 0)
            {
                trocaConteudo(at, at->prox);
            }
            at = at->prox;
        }
    }
}

// Função de comparação para o qsort
int compararListas(const void *a, const void *b)
{
    Lista *listaA = (Lista *)a;
    Lista *listaB = (Lista *)b;

    // Tratamento para listas vazias (segurança)
    if (listaA->inicio == NULL && listaB->inicio == NULL)
        return 0;
    if (listaA->inicio == NULL)
        return 1; // Lista vazia vai para o final
    if (listaB->inicio == NULL)
        return -1;

    // Compara o nome do usuário "Dono" da lista (quem segue) se TipoGrafo == 0
    // e quem e seguido se TipoGrafo == 1
    if (TipoGrafo == 0)
        return strcmp(listaA->inicio->nomeUsuarioQueSegue, listaB->inicio->nomeUsuarioQueSegue);
    return strcmp(listaA->inicio->nomeUsuarioQueESeguida, listaB->inicio->nomeUsuarioQueESeguida);
}

void ordenarListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas, int Tipo)
{
    TipoGrafo = Tipo;
    qsort(ListaAdjacencia, qtdPessoas, sizeof(Lista), compararListas);
    for (int i = 0; i < qtdPessoas; i++)
    {
        ordenarListaInterna(ListaAdjacencia[i]);
    }
}

Lista *criaListaAdjacencia(FILE *arqP, FILE *arqS, RegistroIndice **DadosIndice, int qtdPessoas, int qtdSegues)
{
    Lista *ListaDeAdjacencia = (Lista *)malloc(sizeof(Lista) * qtdPessoas);

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
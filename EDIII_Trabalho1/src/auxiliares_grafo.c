#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#define MAXIMO 2000

void imprimeListaAdjacencia(Adjacentes *ListaAdjacencia, int qtdPessoas)
{
    for(int i = 0; i < qtdPessoas; i++){
        int qtdSegue = ListaAdjacencia[i].quantidadeLigadas;
        for(int j = 0; j < qtdSegue; j++){
            printf("%s, " ,ListaAdjacencia[i].encadeadas[j].nomeUsuarioQueSegue);
            printf("%s, " ,ListaAdjacencia[i].encadeadas[j].nomeUsuarioQueESeguida);
            if(strcmp(ListaAdjacencia[i].encadeadas[j].dataInicioQueSegue, "-") != 0)
            printf("%s, " ,ListaAdjacencia[i].encadeadas[j].dataInicioQueSegue);
            else printf("NULO, ");
            if(strcmp(ListaAdjacencia[i].encadeadas[j].dataFimQueSegue, "-") != 0)
            printf("%s, " ,ListaAdjacencia[i].encadeadas[j].dataFimQueSegue);
            else printf("NULO, ");
            if(ListaAdjacencia[i].encadeadas[j].grauAmizade != '-')
            printf("%s" ,ListaAdjacencia[i].encadeadas[j].grauAmizade);
            else printf("NULO");
            printf("\n");
        }
        printf("\n");
    }
}


static int OrdenaAdjacencia(const void *a, const void *b)
{
    // 1. Cast para Adjacentes (porque estamos ordenando o array principal)
    const Adjacentes *adjA = (const Adjacentes *)a;
    const Adjacentes *adjB = (const Adjacentes *)b;

    // SEGURANÇA: Verificar se existem nós encadeados.
    // Se a lista estiver vazia, não temos nome para comparar.
    // Tratamento simples: vazios vão para o final.
    if (adjA->quantidadeLigadas == 0 && adjB->quantidadeLigadas == 0) return 0;
    if (adjA->quantidadeLigadas == 0) return 1; 
    if (adjB->quantidadeLigadas == 0) return -1;

    // 2. Acessar os dados do PRIMEIRO nó da lista para usar como referência
    const No *noA = &adjA->encadeadas[0];
    const No *noB = &adjB->encadeadas[0];

    // 3. Critério Principal: Nome (Ordem alfabética)
    int comparacaoNome = strcmp(noA->nomeUsuarioQueSegue, noB->nomeUsuarioQueSegue);

    if (comparacaoNome != 0) {
        return comparacaoNome;
    }

    // 4. Critério de Desempate: Data
    int diaA, mesA, anoA;
    int diaB, mesB, anoB;

    sscanf(noA->dataInicioQueSegue, "%d/%d/%d", &diaA, &mesA, &anoA);
    sscanf(noB->dataInicioQueSegue, "%d/%d/%d", &diaB, &mesB, &anoB);

    if (anoA != anoB) return anoA - anoB;
    if (mesA != mesB) return mesA - mesB;
    return diaA - diaB;
}


void InsereAdjacencia(FILE *arqP, FILE *arqS, int Map[], Adjacentes *listaAdjacencia, int qtdSegues, int qtdPessoas, RegistroIndice **DadosIndice)
{
    fseek(arqS, SEEK_SET, 9);
    int Contador = 0;
    while (Contador < qtdSegues)
    {
        RegistroSegue RegAtualS;
        leRegistroSegue(arqS, &RegAtualS);
        if(RegAtualS.removido = '1') 
        {
            fseek(arqS, SEEK_CUR, 29);
            continue;
        }


        int Indice = Map[RegAtualS.idPessoaQueSegue];
        int TamanhoAtual = listaAdjacencia[Indice].quantidadeLigadas;

        No NoAtual = listaAdjacencia[Indice].encadeadas[TamanhoAtual];

        long Offset = buscaBinariaIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueSegue);
        fseek(arqP, SEEK_SET, Offset);
        RegistroPessoa *RegAtualP = leRegistroPessoa(arqP);
        NoAtual.nomeUsuarioQueSegue = malloc(MAXIMO);
        strcpy(NoAtual.nomeUsuarioQueSegue,RegAtualP->nomePessoa);

        Offset = buscaBinariaIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueESeguida);
        fseek(arqP, SEEK_SET, Offset);
        RegAtualP = leRegistroPessoa(arqP); 
        NoAtual.nomeUsuarioQueESeguida = malloc(MAXIMO);
        strcpy(NoAtual.nomeUsuarioQueESeguida, RegAtualS.idPessoaQueESeguida);

        strcpy(NoAtual.dataInicioQueSegue,RegAtualS.dataInicioQueSegue);

        strpcy(NoAtual.dataFimQueSegue, RegAtualS.dataFimQueSegue);

        NoAtual.grauAmizade = RegAtualS.grauAmizade;

        listaAdjacencia[Indice].encadeadas[TamanhoAtual] = NoAtual;
        listaAdjacencia[Indice].quantidadeLigadas++;

        Contador++;
    }
}

Adjacentes *criaListaAdjacencia(FILE *arqP, FILE *arqS, RegistroIndice **DadosIndice, int qtdPessoas, int qtdSegues)
{
    Adjacentes *ListaAdjacencia = (Adjacentes *)malloc(qtdPessoas * sizeof(Adjacentes));

    int Map[MAXIMO];
    memset(Map, -1, sizeof(Map));

    for (int i = 0; i < qtdPessoas; i++)
    {
        Map[DadosIndice[i]->idPessoa] = i;

        ListaAdjacencia[i].quantidadeLigadas = 0;
        ListaAdjacencia[i].encadeadas = (No *)malloc(qtdSegues * sizeof(No));
    }
    InsereAdjacencia(arqP, arqS, Map, ListaAdjacencia, qtdSegues, qtdPessoas, DadosIndice);
    qsort(ListaAdjacencia, qtdPessoas, sizeof(Adjacentes), OrdenaAdjacencia);
    return ListaAdjacencia;
}
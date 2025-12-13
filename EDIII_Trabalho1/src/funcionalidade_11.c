#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "auxiliares_grafo.h"
#include "io_cabecalho.h"
#include "utilidades.h"
#define MAXIMO 2000

void liberaListaAdjacencia(Lista *lista, int qtdPessoas)
{
    if (lista == NULL) return;

    //Percorre cada posição do vetor principal (cada vértice)
    for (int i = 0; i < qtdPessoas; i++)
    {
        No *atual = lista[i].inicio;
        No *proxNo;

        //Percorre a lista encadeada de arestas
        while (atual != NULL)
        {
            proxNo = atual->prox; // Salva o endereço do próximo antes de destruir o atual
            
           
            // liberamos a estrutura do nó.
            free(atual);
            
            atual = proxNo;
        }
        
        // Zera os ponteiros da lista (boa prática)
        lista[i].inicio = NULL;
        lista[i].fim = NULL;
        lista[i].tamanho = 0;
    }

    //Libera o vetor principal de listas
    free(lista);
}

int buscaAmizades(char *arqPessoa, char *arqIndex, char *arqSegue)
{
    // Abre os arquivos e verifica se conseguiu abrir
    FILE *arquivoPessoa = fopen(arqPessoa, "rb");
    FILE *arquivoIndice = fopen(arqIndex, "rb");
    FILE *arquivoSegue = fopen(arqSegue, "rb");
    if (arquivoPessoa == NULL || arquivoIndice == NULL || arquivoSegue == NULL)
    {
        return 0;
    }
    int TipoGrafo = 0;
    // Le o cabecalho do arquivo pessoa
    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arquivoPessoa);

    // Le o cabecalho do arquivo segue
    CabecalhoSegue CabecalhoS = leCabecalhoSegue(arquivoSegue);

    // Passa todos os dados do arquivo de Indice para a RAM
    RegistroIndice *DadosIndice = leArqIndice(arquivoIndice, CabecalhoP->quantidadePessoas);

    // Cria a lista de adjacência na RAM
    Lista *ListaAdjacencia = criaListaAdjacencia(TipoGrafo, arquivoPessoa , arquivoSegue, DadosIndice, CabecalhoP->quantidadePessoas, CabecalhoS.quantidadePessoas);
    // Ordena lista de adjacência
    ordenarListaAdjacencia(ListaAdjacencia, CabecalhoP->quantidadePessoas);

    // Imprime a lista de adjacência
    imprimeListaAdjacencia(ListaAdjacencia, CabecalhoP->quantidadePessoas);

    // Libera a memória alocada
    liberaListaAdjacencia(ListaAdjacencia, CabecalhoP->quantidadePessoas);
    fclose(arquivoPessoa);
    fclose(arquivoIndice);
    fclose(arquivoSegue);
    free(CabecalhoP);
    free(DadosIndice);
    return 1;
}

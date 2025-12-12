#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "auxiliares_grafo.h"
#include "io_cabecalho.h"
#include "utilidades.h"
#define MAXIMO 2000

void liberaRegistroIndice(RegistroIndice **DadosIndice, int qtdPessoas)
{
    for (int i = 0; i < qtdPessoas; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
}
void liberaListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    for (int i = 0; i < qtdPessoas; i++)
    {
        int qtdSegue = ListaAdjacencia[i].tamanho;
        No *at = ListaAdjacencia[i].inicio;
        for (int j = 0; j < qtdSegue; j++)
        {
            No *prox = at->prox;
            free(at->nomeUsuarioQueESeguida);
            free(at->nomeUsuarioQueSegue);
            free(at); 
            at = prox; 
        }
    }
    free(ListaAdjacencia);
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

    // Define o tipo de Grafo, 0 = Normal, 1 = Transposto
    int TipoGrafo = 1;
    // Le o cabecalho do arquivo pessoa
    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arquivoPessoa);
    // Le o cabecalho do arquivo segue
    CabecalhoSegue CabecalhoS = leCabecalhoSegue(arquivoSegue);

    // Passa todos os dados do arquivo de Indice para a RAM
    RegistroIndice **DadosIndice = leArquivoIndice(arquivoIndice, CabecalhoP->quantidadePessoas);

    // Cria a lista de adjacência na RAM
    Lista *ListaAdjacencia = criaListaAdjacencia(arquivoPessoa, arquivoSegue, DadosIndice, CabecalhoP->quantidadePessoas, CabecalhoS.quantidadePessoas);

    // Ordena lista de adjacência
    ordenarListaAdjacencia(ListaAdjacencia, CabecalhoP->quantidadePessoas, TipoGrafo);

    // Imprime a lista de adjacência
    imprimeListaAdjacencia(ListaAdjacencia, CabecalhoP->quantidadePessoas);

    // Libera a memória alocada
    liberaListaAdjacencia(ListaAdjacencia, CabecalhoP->quantidadePessoas);
    liberaRegistroIndice(DadosIndice, CabecalhoP->quantidadePessoas);
    free(CabecalhoP);
    fclose(arquivoPessoa);
    fclose(arquivoIndice);
    fclose(arquivoSegue);
    return 1;
}

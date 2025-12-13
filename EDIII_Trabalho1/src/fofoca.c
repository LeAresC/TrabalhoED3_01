#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fofoca.h"
#include "auxiliares_busca.h"
#include "auxiliares_grafo.h"
#include "busca_grafo.h"
#include "io_cabecalho.h"
#include "funcionalidade_11.h"

void fofoca(char *arqPessoa, char *arqIndex, char *arqSegue, char *nomeFofoqueiro) {
    FILE *arquivoPessoa, *arquivoIndice, *arquivoSegue;
    CabecalhoPessoa *cabecalhoP;
    CabecalhoSegue cabecalhoS;
    RegistroIndice *dadosIndice;
    Lista *listaAdjacencia;
    int tipoGrafo, indiceFofoqueiro, cicloLen;
    
    // Abre os arquivos e verifica se conseguiu abrir
    arquivoPessoa = fopen(arqPessoa, "rb");
    arquivoIndice = fopen(arqIndex, "rb");
    arquivoSegue = fopen(arqSegue, "rb");
    if (arquivoPessoa == NULL || arquivoIndice == NULL || arquivoSegue == NULL) {
        if (arquivoPessoa) fclose(arquivoPessoa);
        if (arquivoIndice) fclose(arquivoIndice);
        if (arquivoSegue) fclose(arquivoSegue);
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Define o tipo de grafo como normal (não transposto)
    tipoGrafo = 0;
    
    // Le os cabeçalhos dos arquivos
    cabecalhoP = leCabecalhoPessoa(arquivoPessoa);
    cabecalhoS = leCabecalhoSegue(arquivoSegue);
    
    // Carrega o índice completo para a RAM
    dadosIndice = leArqIndice(arquivoIndice, cabecalhoP->quantidadePessoas);
    
    // Cria a lista de adjacência
    listaAdjacencia = criaListaAdjacencia(tipoGrafo, arquivoPessoa, arquivoSegue, dadosIndice, 
                                          cabecalhoP->quantidadePessoas, cabecalhoS.quantidadePessoas);
    
    // Preenche o campo nomeDono (ponte entre Feature 11 e 14)
    preencherNomesDono(listaAdjacencia, cabecalhoP->quantidadePessoas, dadosIndice, arquivoPessoa);
    
    // Ordena a lista de adjacência
    ordenarListaAdjacencia(listaAdjacencia, cabecalhoP->quantidadePessoas);
    
    // Busca o índice do fofoqueiro no grafo
    indiceFofoqueiro = buscarIndice(listaAdjacencia, cabecalhoP->quantidadePessoas, nomeFofoqueiro);
    
    if (indiceFofoqueiro == -1) {
        printf("Falha no processamento do arquivo.\n");
        liberarNomesDono(listaAdjacencia, cabecalhoP->quantidadePessoas);
        liberaListaAdjacencia(listaAdjacencia, cabecalhoP->quantidadePessoas);
        free(cabecalhoP);
        free(dadosIndice);
        fclose(arquivoPessoa);
        fclose(arquivoIndice);
        fclose(arquivoSegue);
        return;
    }
    
    // Executa BFS para detectar ciclo
    cicloLen = executarBFSCiclo(listaAdjacencia, cabecalhoP->quantidadePessoas, indiceFofoqueiro);
    
    // Verifica se encontrou ciclo
    if (cicloLen > 0) {
        printf("%d\n", cicloLen);
    } else {
        printf("A FOFOCA NAO RETORNOU\n");
    }
    
    // Libera toda a memória alocada
    liberarNomesDono(listaAdjacencia, cabecalhoP->quantidadePessoas);
    liberaListaAdjacencia(listaAdjacencia, cabecalhoP->quantidadePessoas);
    free(cabecalhoP);
    free(dadosIndice);
    fclose(arquivoPessoa);
    fclose(arquivoIndice);
    fclose(arquivoSegue);
}

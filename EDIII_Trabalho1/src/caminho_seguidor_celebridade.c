#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caminho_seguidor_celebridade.h"
#include "auxiliares_busca.h"
#include "auxiliares_grafo.h"
#include "busca_grafo.h"
#include "io_cabecalho.h"
#include "funcionalidade_11.h"

void caminhoSeguidorCelebridade(char *arqPessoa, char *arqIndex, char *arqSegue, char *nomeCelebridade) {
    FILE *arquivoPessoa, *arquivoIndice, *arquivoSegue;
    CabecalhoPessoa *cabecalhoP;
    CabecalhoSegue cabecalhoS;
    RegistroIndice *dadosIndice;
    Lista *listaAdjacencia;
    int tipoGrafo, indiceCelebridade, i;
    ResultadoBFS *resultado;
    
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
    
    // Preenche o campo nomeDono (ponte entre Feature 11 e 13)
    preencherNomesDono(listaAdjacencia, cabecalhoP->quantidadePessoas, dadosIndice, arquivoPessoa);
    
    // Ordena a lista de adjacência
    ordenarListaAdjacencia(listaAdjacencia, cabecalhoP->quantidadePessoas);
    
    // Busca o índice da celebridade no grafo
    indiceCelebridade = buscarIndice(listaAdjacencia, cabecalhoP->quantidadePessoas, nomeCelebridade);
    
    if (indiceCelebridade == -1) {
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
    
    // Itera por todos os usuários em ordem alfabética
    for (i = 0; i < cabecalhoP->quantidadePessoas; i++) {
        // Pula se for a própria celebridade
        if (i == indiceCelebridade) continue;
        
        // Executa BFS para encontrar caminho de i até a celebridade
        resultado = executarBFSCaminho(listaAdjacencia, cabecalhoP->quantidadePessoas, i, indiceCelebridade);
        
        // Verifica se existe caminho
        if (resultado->distancia[indiceCelebridade] != -1) {
            // Imprime o caminho recursivamente
            imprimirCaminhoRecursivo(listaAdjacencia, resultado->antecessor, indiceCelebridade, i);
            printf("\n");
        }
        
        // Libera resultado do BFS
        liberarResultadoBFS(resultado);
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "busca_grafo.h"
#include "auxiliares_grafo.h"

int buscarIndice(Lista *grafo, int numVertices, char *nome) {
    int esq, dir, meio, cmp;
    
    esq = 0;
    dir = numVertices - 1;
    
    // Busca binária no array de listas ordenado por nomeDono
    while (esq <= dir) {
        meio = (esq + dir) / 2;
        
        if (grafo[meio].nomeDono == NULL) return -1;
        
        cmp = strcmp(grafo[meio].nomeDono, nome);
        
        if (cmp == 0) return meio;
        if (cmp < 0) esq = meio + 1;
        else dir = meio - 1;
    }
    
    return -1;
}

Fila* criarFila() {
    Fila *f;
    
    // Aloca e inicializa fila vazia
    f = (Fila*) malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    
    return f;
}

void enfileirar(Fila *f, int idx) {
    NoFila *novo;
    
    // Cria novo nó da fila
    novo = (NoFila*) malloc(sizeof(NoFila));
    novo->verticeIdx = idx;
    novo->prox = NULL;
    
    // Adiciona no final da fila
    if (f->fim != NULL) {
        f->fim->prox = novo;
    } else {
        f->inicio = novo;
    }
    f->fim = novo;
}

int desenfileirar(Fila *f) {
    NoFila *temp;
    int val;
    
    if (f->inicio == NULL) return -1;
    
    // Remove do início da fila
    temp = f->inicio;
    val = temp->verticeIdx;
    f->inicio = temp->prox;
    
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    
    free(temp);
    return val;
}

int filaVazia(Fila *f) {
    return (f->inicio == NULL);
}

void liberarFila(Fila *f) {
    // Esvazia a fila removendo todos os elementos
    while (!filaVazia(f)) {
        desenfileirar(f);
    }
    free(f);
}

ResultadoBFS* criarResultadoBFS(int numVertices) {
    ResultadoBFS *res;
    int i;
    
    // Aloca estrutura de resultado
    res = (ResultadoBFS*) malloc(sizeof(ResultadoBFS));
    res->antecessor = (int*) malloc(numVertices * sizeof(int));
    res->distancia = (int*) malloc(numVertices * sizeof(int));
    res->visitado = (int*) calloc(numVertices, sizeof(int));
    res->tamanho = numVertices;
    
    // Inicializa arrays
    for(i = 0; i < numVertices; i++) {
        res->antecessor[i] = -1;
        res->distancia[i] = -1;
    }
    
    return res;
}

void liberarResultadoBFS(ResultadoBFS *res) {
    if(res) {
        free(res->antecessor);
        free(res->distancia);
        free(res->visitado);
        free(res);
    }
}

ResultadoBFS* executarBFSCaminho(Lista *grafo, int numVertices, int startIdx, int targetIdx) {
    ResultadoBFS *res;
    Fila *f;
    int u, v;
    No *aresta;
    
    // Cria estruturas para o BFS
    res = criarResultadoBFS(numVertices);
    f = criarFila();
    
    // Inicializa vértice de partida
    res->visitado[startIdx] = 1;
    res->distancia[startIdx] = 0;
    enfileirar(f, startIdx);
    
    // Processa fila BFS
    while (!filaVazia(f)) {
        u = desenfileirar(f);
        
        if (u == targetIdx) break;
        
        // Percorre lista de adjacência do vértice u
        aresta = grafo[u].inicio;
        while (aresta != NULL) {
            // Verifica se a relação ainda está ativa (dataFimQueSegue vazio)
            if (aresta->dataFimQueSegue[0] == '\0') {
                v = buscarIndice(grafo, numVertices, aresta->nomeUsuarioQueESeguida);
                
                if (v != -1) {
                    int newDist = res->distancia[u] + 1;
                    
                    // Se não visitado ainda, ou se mesma distância mas caminho lexicograficamente melhor
                    if (!res->visitado[v]) {
                        res->visitado[v] = 1;
                        res->distancia[v] = newDist;
                        res->antecessor[v] = u;
                        enfileirar(f, v);
                    }
                    // Se já visitado com mesma distância, verifica se este caminho é lexicograficamente melhor
                    else if (res->distancia[v] == newDist) {
                        // Compara o predecessor imediato (vértice que vem antes de v no caminho)
                        if (strcmp(grafo[u].nomeDono, grafo[res->antecessor[v]].nomeDono) < 0) {
                            res->antecessor[v] = u;
                            enfileirar(f, v);
                        }
                    }
                }
            }
            
            aresta = aresta->prox;
        }
    }
    
    liberarFila(f);
    return res;
}

void imprimirCaminhoRecursivo(Lista *grafo, int *antecessor, int atualIdx, int startIdx) {
    int paiIdx;
    No *aresta;
    char *fim;
    char grau;
    
    if (atualIdx == startIdx) return;
    
    paiIdx = antecessor[atualIdx];
    if (paiIdx == -1) return;
    
    // Imprime caminho recursivamente até o início
    imprimirCaminhoRecursivo(grafo, antecessor, paiIdx, startIdx);
    
    // Busca a aresta que conecta paiIdx a atualIdx
    aresta = grafo[paiIdx].inicio;
    while (aresta != NULL) {
        if (strcmp(aresta->nomeUsuarioQueESeguida, grafo[atualIdx].nomeDono) == 0) {
            
            // Trata valores NULO para dataFim
            if (strcmp(aresta->dataFimQueSegue, "") == 0 || strcmp(aresta->dataFimQueSegue, "-") == 0) {
                fim = "NULO";
            } else {
                fim = aresta->dataFimQueSegue;
            }
            
            grau = aresta->grauAmizade;
            
            // Imprime aresta no formato especificado
            if (grau == '$' || grau == '\0') {
                printf("%s, %s, %s, %s, NULO\n", 
                    grafo[paiIdx].nomeDono, 
                    grafo[atualIdx].nomeDono, 
                    aresta->dataInicioQueSegue, 
                    fim);
            } else {
                printf("%s, %s, %s, %s, %c\n", 
                    grafo[paiIdx].nomeDono, 
                    grafo[atualIdx].nomeDono, 
                    aresta->dataInicioQueSegue, 
                    fim, 
                    grau);
            }
            break;
        }
        aresta = aresta->prox;
    }
}

int executarBFSCiclo(Lista *grafo, int numVertices, int startIdx) {
    Fila *f;
    int *distancia;
    int u, v, cicloLen;
    No *aresta;
    int i;
    
    // Prepara estruturas para BFS
    f = criarFila();
    distancia = (int*) malloc(numVertices * sizeof(int));
    
    for(i = 0; i < numVertices; i++) {
        distancia[i] = -1;
    }
    
    // Inicia BFS do vértice startIdx
    distancia[startIdx] = 0;
    enfileirar(f, startIdx);
    
    // Processa fila procurando ciclo
    while (!filaVazia(f)) {
        u = desenfileirar(f);
        
        aresta = grafo[u].inicio;
        while (aresta != NULL) {
            // For cycle detection, check ALL edges (active and inactive)
            v = buscarIndice(grafo, numVertices, aresta->nomeUsuarioQueESeguida);
            
            if (v != -1) {
                // Verifica se encontrou ciclo de volta ao início
                if (v == startIdx) {
                    cicloLen = distancia[u] + 1;
                    free(distancia);
                    liberarFila(f);
                    return cicloLen;
                }
                
                // Continua BFS normalmente
                if (distancia[v] == -1) {
                    distancia[v] = distancia[u] + 1;
                    enfileirar(f, v);
                }
            }
            aresta = aresta->prox;
        }
    }
    
    // Nenhum ciclo encontrado
    free(distancia);
    liberarFila(f);
    return -1;
}

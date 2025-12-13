#ifndef BUSCA_GRAFO_H
#define BUSCA_GRAFO_H

#include "data_structs.h"

// Estrutura de nó da fila para BFS
typedef struct NoFila {
    int verticeIdx;
    struct NoFila *prox;
} NoFila;

// Estrutura da fila
typedef struct {
    NoFila *inicio;
    NoFila *fim;
} Fila;

// Estrutura de resultado do BFS
typedef struct {
    int *antecessor;
    int *distancia;
    int *visitado;
    int tamanho;
} ResultadoBFS;

// Funções auxiliares
int buscarIndice(Lista *grafo, int numVertices, char *nome);

// Operações de fila
Fila* criarFila();
void enfileirar(Fila *f, int idx);
int desenfileirar(Fila *f);
int filaVazia(Fila *f);
void liberarFila(Fila *f);

// BFS para caminho mais curto (Funcionalidade 13)
ResultadoBFS* executarBFSCaminho(Lista *grafo, int numVertices, int startIdx, int targetIdx);
void liberarResultadoBFS(ResultadoBFS *res);
void imprimirCaminhoRecursivo(Lista *grafo, int *antecessor, int atualIdx, int startIdx);

// BFS para detecção de ciclos (Funcionalidade 14)
int executarBFSCiclo(Lista *grafo, int numVertices, int startIdx);

#endif

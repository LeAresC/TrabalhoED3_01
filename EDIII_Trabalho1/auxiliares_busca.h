#ifndef AUXILIARES_BUSCA_H
#define AUXILIARES_BUSCA_H
#include <stdio.h>
#include "data_structs.h"

void erroAbertura();
void erroRegistro();
void imprimirSaida(RegistroPessoa *registroAtual);
int lerArquivoBinario(char *nomeDoArquivo);
int buscaEmArquivo(char *arquivoDados, char *arquivoIndice,int qtd);
RegistroPessoa *leRegistro(FILE *arq);
int buscaBinariaIndice(FILE *arqI, int valorCampo);
CabecalhoPessoa *leCabecalho(FILE *arq);
void scanQuoteString(char *str);
void leCriterioBusca(char *campo, char *valor);
RegistroPessoa** buscaPessoas(FILE *arquivoPessoa, RegistroIndice *registroIndice, int quantidadeIndices,
                              CabecalhoPessoa *cabecalhoPessoa, char *nomeCampo, char *valorCampo, int *counter);

#endif // AUXILIARES_BUSCA_H
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
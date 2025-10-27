#ifndef AUXILIARES_BUSCA_H
#define AUXILIARES_BUSCA_H
#include <stdio.h>
#include "data_structs.h"

void erroAbertura();
void erroRegistro();
void imprimirSaida(RegistroPessoa *registroAtual);
int lerArquivoBinario(char *nomeDoArquivo);
RegistroPessoa **buscaDados(FILE *arqD,RegistroIndice **DadosIndice, CabecalhoPessoa *Cabecalho);
RegistroPessoa *leRegistro(FILE *arq);
RegistroIndice **leArquivoIndice(FILE *arqI, int qtdDados);
CabecalhoIndice *leCabecalhoIndice(FILE *arqI);
int buscaBinariaIndice(RegistroIndice** ArquivoIndice, int tamanhoVetor, int valorCampo);
CabecalhoPessoa *leCabecalho(FILE *arq);
void scanQuoteString(char *str);

#endif // AUXILIARES_BUSCA_H
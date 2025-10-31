#ifndef AUXILIARES_BUSCA_H
#define AUXILIARES_BUSCA_H
#include <stdio.h>
#include "data_structs.h"

RegistroIndice **leArquivoIndice(FILE *arqI, int N);
RegistroPessoa *leRegistroPessoa(FILE *arq);
CabecalhoIndice *leCabecalhoIndice(FILE *arqI);
CabecalhoPessoa *leCabecalhoPessoa(FILE *arqD);
void erroAbertura();
void erroRegistro();
void imprimirSaida(RegistroPessoa *registroAtual);
void scanQuoteString(char *str);
long *buscaDados(FILE *arqD,RegistroIndice **DadosIndice, char *nomeCampo, char *valorCampo);
int buscaBinariaIndice(RegistroIndice** ArquivoIndice, int tamanhoVetor, int valorCampo);
void descartaLixo(FILE *arqD);
void leInput(char *nomeCampo, char *valorCampo);
#endif // AUXILIARES_BUSCA_H
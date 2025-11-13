#ifndef IO_CABECALHO_H
#define IO_CABECALHO_H

#include <stdio.h>
#include "data_structs.h"

void escreveCabecalhoIndice(FILE *arquivoIndice, char status);
void escreveCabecalhoPessoa(FILE *arquivoPessoa, char status, int quantidadePessoas, int quantidadeRemovidos, long proxByteOffset);
void escreveCabecalhoSegue(FILE *arquivoSegue, char status, int quantidadePessoas, int proxRRN);
CabecalhoSegue leCabecalhoSegue(FILE *arquivoSegue);

#endif // IO_CABECALHO_H
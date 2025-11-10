#ifndef ESCREVE_CABECALHO_H
#define ESCREVE_CABECALHO_H

#include <stdio.h>
#include "data_structs.h"

void escreveCabecalhoIndice(FILE *arquivoIndice, char status);
void escreveCabecalhoPessoa(FILE *arquivoPessoa, char status, int quantidadePessoas, int quantidadeRemovidos, long long int proxByteOffset);
void escreveCabecalhoSegue(FILE *arquivoSegue, char status, int quantidadePessoas, long long int proxRRN);
CabecalhoSegue leCabecalhoSegue(FILE *arquivoSegue);
CabecalhoPessoa leCabecalhoPessoa(FILE *arquivoPessoa);

#endif // ESCREVE_CABECALHO_H
#ifndef CRIAR_TABELA_H
#define CRIAR_TABELA_H

#include "data_structs.h"
#include <stdio.h>

int criarTabela(const char *nomeArquivoCsv, const char *nomeArquivoDados, const char *nomeArquivoIndice);

RegistroPessoa* lerRegistroCsv(FILE *arquivoCsv);

#endif // CRIAR_TABELA_H

#ifndef CRIAR_TABELA_H
#define CRIAR_TABELA_H

#include "data_structs.h"
#include <stdio.h>

int criarTabela(char *nomeCSV, char *nomeData, char *nomeIndex);

RegistroPessoa* lerRegistroCsv(FILE *arquivoCsv);

#endif // CRIAR_TABELA_H

#ifndef UTILS_BUSCA_H
#define UTILS_BUSCA_H

#include <stdio.h>
#include "data_structs.h"

int compararRegistrosIndice(const void *a, const void *b);
int compararRegistroSegue(const void *a, const void *b);
RegistroPessoa** buscaPessoas(FILE *arquivoPessoa, RegistroIndice *registroIndice, int quantidadeIndices,
                              char *nomeCampo, char *valorCampo, int *counter);

#endif // UTILS_BUSCA_H

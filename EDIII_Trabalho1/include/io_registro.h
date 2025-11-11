#ifndef IO_REGISTRO_H
#define IO_REGISTRO_H

#include <stdio.h>
#include "data_structs.h"

void escreveRegistroPessoa(FILE *arquivo, RegistroPessoa *registro);
void escreveRegistroSegue(FILE *arquivo, RegistroSegue *registro);
void escreveRegistroIndice(FILE *arquivo, RegistroIndice *registro);
RegistroPessoa* leRegistroPessoa(FILE *arquivo);
void leRegistroSegue(FILE *arquivo, RegistroSegue *registro);
void leRegistroIndice(FILE *arquivo, RegistroIndice *registro);
void liberaRegistroPessoa(RegistroPessoa *registro);

#endif // IO_REGISTRO_H

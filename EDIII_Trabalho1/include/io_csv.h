#ifndef IO_CSV_H
#define IO_CSV_H

#include <stdio.h>
#include "data_structs.h"

RegistroPessoa* leRegistroPessoaCsv(FILE *arquivoCsv);
RegistroSegue* leRegistroSegueCsv(FILE *arquivoCsv);

#endif // IO_CSV_H

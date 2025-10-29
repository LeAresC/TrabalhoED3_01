#include "data_structs.h"

int escreveIndice(char *arquivoIndice, RegistroIndice **DadosIndice, int tamanhoArquivoIndice);
int compararIndicePorID(const void *a, const void *b);
void removeRegistroOffset(FILE *arqD, long ByteOffset, RegistroIndice **DadosIndice, int tamanhoIndice);
void insereFinal(FILE *arqD, RegistroPessoa *RegistroAtual,RegistroIndice **DadosIndice, int tamanhoIndice, long offset);
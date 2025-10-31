#include "data_structs.h"

void escreveIndice(char *arquivoIndice, RegistroIndice **DadosIndice, int tamanhoArquivoIndice);
int compararIndicePorID(const void *a, const void *b);
void removeRegistroOffsetPessoa(FILE *arqD, long ByteOffset, RegistroPessoa *RegistroAtual);
void insereFinalPessoa(FILE *arqD, RegistroPessoa *RegistroAtual, long Offset);
void insereMeioPessoa(FILE *arqD, long ByteOffset, RegistroPessoa *RegistroAtual);
void insereFinalIndice(RegistroIndice **DadosIndice, int tamanhoIndice, int ID,long byteOffset);
void removeRegistroOffsetIndice(RegistroIndice **DadosIndice, int tamanhoIndice, int ID);
void atualizaNoIndice(RegistroIndice **DadosIndice,int tamanhoIndice, int IdAtual, int Offset, int IdAnterior);
void atualizaCabecalhoPessoa(FILE *arqD , CabecalhoPessoa *CabecalhoP);
void insereLixoFinalRegistro(FILE *arqD, int QuantidadeDeLixo);
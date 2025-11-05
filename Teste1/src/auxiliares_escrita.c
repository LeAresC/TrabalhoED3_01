#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "auxiliares_busca.h"
#define MAXIMO 2000
#define INF 1e9

void insereLixoFinalRegistro(FILE *arqD, int QuantidadeDeLixo)
{
    char Lixo = '$';
    for (int i = 0; i < QuantidadeDeLixo; i++)
    {
        fwrite(&Lixo, sizeof(char), 1, arqD);
    }
}

// Função de comparação para um array de PONTEIROS (RegistroIndice **)
int compararIndicePorID(const void *a, const void *b)
{

    // 1. Converte 'a' e 'b' de volta para seus tipos reais:
    //    ponteiros para ponteiros (RegistroIndice **)
    const RegistroIndice **ptrA = (const RegistroIndice **)a;
    const RegistroIndice **ptrB = (const RegistroIndice **)b;

    // 2. Obtém os ponteiros para as structs reais
    //    (dereferenciando uma vez)
    const RegistroIndice *regA = *ptrA;
    const RegistroIndice *regB = *ptrB;

    // 3. Agora sim, compara os campos das structs
    if (regA->idPessoa < regB->idPessoa)
    {
        return -1;
    }
    else if (regA->idPessoa > regB->idPessoa)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void escreveIndice(char *arquivoIndice, RegistroIndice **DadosIndice, int tamanhoArquivoIndice)
{
    FILE *arqI = fopen(arquivoIndice, "wb");
    CabecalhoIndice Cabecalho;
    Cabecalho.status = '1';
    memset(Cabecalho.lixo, '$', sizeof(Cabecalho.lixo));
    fwrite(&Cabecalho.status, sizeof(Cabecalho.status), 1, arqI);
    fwrite(Cabecalho.lixo, sizeof(Cabecalho.lixo), 1, arqI);

    for (int i = 0; i < tamanhoArquivoIndice; i++)
    {
        fwrite(&DadosIndice[i]->idPessoa, sizeof(int), 1, arqI);
        fwrite(&DadosIndice[i]->byteOffset, sizeof(long long int), 1, arqI);
    }
    fclose(arqI);
}

void removeRegistroOffsetPessoa(FILE *arqD, long ByteOffset)
{
    fseek(arqD, ByteOffset, SEEK_SET);
    char aux = '1';
    // Apaga do Arquivo Pessoa
    fwrite(&aux, sizeof(char), 1, arqD);
}
void inserePessoa(FILE *arqD, long ByteOffset, RegistroPessoa *RegistroAtual)
{
    fseek(arqD, ByteOffset, SEEK_SET);
    fwrite(&RegistroAtual->removido, sizeof(char), 1, arqD);
    fwrite(&RegistroAtual->tamanhoRegistro, sizeof(int), 1, arqD);
    fwrite(&RegistroAtual->idPessoa, sizeof(int), 1, arqD);
    fwrite(&RegistroAtual->idadePessoa, sizeof(int), 1, arqD);
    fwrite(&RegistroAtual->tamanhoNomePessoa, sizeof(int), 1, arqD);
    fwrite(RegistroAtual->nomePessoa, sizeof(char), RegistroAtual->tamanhoNomePessoa, arqD);
    fwrite(&RegistroAtual->tamanhoNomeUsuario, sizeof(int), 1, arqD);
    fwrite(RegistroAtual->nomeUsuario, sizeof(char), RegistroAtual->tamanhoNomeUsuario, arqD);
    int tamanhoTotal = RegistroAtual->tamanhoRegistro;
    int tamanhoValido = 16 + RegistroAtual->tamanhoNomePessoa + RegistroAtual->tamanhoNomeUsuario;
    insereLixoFinalRegistro(arqD, tamanhoTotal - tamanhoValido);
}
void insereFinalIndice(RegistroIndice **DadosIndice, int tamanhoIndice, int ID, long byteOffset)
{
    DadosIndice[tamanhoIndice]->idPessoa = ID;
    DadosIndice[tamanhoIndice]->byteOffset = byteOffset;
    qsort(DadosIndice, tamanhoIndice, sizeof(RegistroIndice *), compararIndicePorID);
}
void removeRegistroOffsetIndice(RegistroIndice **DadosIndice, int tamanhoIndice, int ID)
{
    // Sinaliza que apagou no arquivo de Indice
    long offsetIndice = buscaBinariaIndice(DadosIndice, tamanhoIndice, ID);
    DadosIndice[offsetIndice]->idPessoa = INF;
    qsort(DadosIndice, tamanhoIndice, sizeof(RegistroIndice *), compararIndicePorID);
}
void atualizaNoIndice(RegistroIndice **DadosIndice, int tamanhoIndice, int idAtual, long Offset, int idAnterior)
{
    int offsetIndice = buscaBinariaIndice(DadosIndice, tamanhoIndice, idAnterior);
    DadosIndice[offsetIndice]->byteOffset = Offset;
    DadosIndice[offsetIndice]->idPessoa = idAtual;
    qsort(DadosIndice, tamanhoIndice, sizeof(RegistroIndice *), compararIndicePorID);
}
void atualizaCabecalhoPessoa(FILE *arqD, CabecalhoPessoa *CabecalhoP)
{
    fseek(arqD, 0, SEEK_SET);
    fwrite(&CabecalhoP->status, sizeof(char), 1, arqD);
    fwrite(&CabecalhoP->quantidadePessoas, sizeof(int), 1, arqD);
    fwrite(&CabecalhoP->quantidadeRemovidos, sizeof(int), 1, arqD);
    fwrite(&CabecalhoP->proxByteOffset, sizeof(long long int), 1, arqD);
}
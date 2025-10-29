#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#include "auxiliares_busca.h"
#define INF 1e9

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
int escreveIndice(char *arquivoIndice, RegistroIndice **DadosIndice, int tamanhoArquivoIndice)
{
    FILE *arqI = fopen(arquivoIndice, "wb");
    if (arqI == NULL)
        return 0;
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
    return 1;
}

void removeRegistroOffset(FILE *arqD, long ByteOffset, RegistroIndice **DadosIndice, int tamanhoIndice)
{
    fseek(arqD, ByteOffset, SEEK_SET);

    RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);

    if (RegistroAtual->removido != '1')
    {

        // Lida com o lixo no inicio do registro
        fseek(arqD, ByteOffset, SEEK_SET);
        long cnt = 0;
        char aux = '$';
        while (aux == '$')
        {
            fread(&aux, sizeof(char), 1, arqD);
            cnt++;
        }
        RegistroAtual->removido = '1';
        fseek(arqD, ByteOffset + cnt - 1, SEEK_SET);
        fwrite(&RegistroAtual->removido, sizeof(char), 1, arqD);

        long offsetIndice = buscaBinariaIndice(DadosIndice, tamanhoIndice, RegistroAtual->idPessoa);
        DadosIndice[offsetIndice]->byteOffset = -1;

        free(RegistroAtual->nomePessoa);
        free(RegistroAtual->nomeUsuario);
    }
    free(RegistroAtual);
}
void insereFinal(FILE *arqD, RegistroPessoa *RegistroAtual, RegistroIndice **DadosIndice, int tamanhoIndice, long offset)
{
    fwrite(&RegistroAtual->removido, sizeof(char), 1, arqD);
    fwrite(&RegistroAtual->tamanhoRegistro, sizeof(int), 1, arqD);
    fwrite(&RegistroAtual->idPessoa, sizeof(int), 1, arqD);
    fwrite(&RegistroAtual->idadePessoa, sizeof(int), 1, arqD);
    fwrite(&RegistroAtual->tamanhoNomePessoa, sizeof(int), 1, arqD);
    fwrite(RegistroAtual->nomePessoa, sizeof(char), RegistroAtual->tamanhoNomePessoa, arqD);
    fwrite(&RegistroAtual->tamanhoNomeUsuario, sizeof(int), 1, arqD);
    fwrite(RegistroAtual->nomeUsuario, sizeof(char), RegistroAtual->tamanhoNomeUsuario, arqD);
    DadosIndice[tamanhoIndice] = malloc(sizeof(RegistroIndice));
    DadosIndice[tamanhoIndice]->idPessoa = RegistroAtual->idPessoa;
    DadosIndice[tamanhoIndice]->byteOffset = offset;
}
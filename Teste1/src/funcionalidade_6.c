#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#define MAXIMO 1000

int insereNoFinal(char *arquivoDados, char *arquivoIndice, int N)
{
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "r+b");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }
    CabecalhoPessoa *Cabecalho = leCabecalhoPessoa(arqD);

    char instavel = '1';
    fwrite(&instavel, sizeof(char), 1, arqI);

    RegistroIndice **DadosIndice = leArquivoIndice(arqI, Cabecalho->quantidadePessoas);

    fseek(arqD, Cabecalho->proxByteOffset, SEEK_SET);
    for (int i = 0; i < N; i++)
    {

        int cnt;

        RegistroPessoa *novoDado = (RegistroPessoa *)malloc(sizeof(RegistroPessoa));
        novoDado->nomePessoa = malloc(sizeof(char) * MAXIMO);
        novoDado->nomeUsuario = malloc(sizeof(char) * MAXIMO);

        novoDado->removido = '0';

        char buffer1[MAXIMO];
        char buffer2[MAXIMO];

        scanf("%d %[^,],", &cnt, buffer1);
        scanQuoteString(novoDado->nomePessoa);
        char aux;
        scanf("%c %[^,],", &aux, buffer2);
        scanQuoteString(novoDado->nomeUsuario);

        novoDado->tamanhoNomePessoa = strlen(novoDado->nomePessoa);
        novoDado->tamanhoNomeUsuario = strlen(novoDado->nomeUsuario);

        novoDado->idPessoa = atoi(buffer1);
        if(strcmp(buffer2, "NULO") != 0)
        novoDado->idadePessoa = atoi(buffer2);
        else novoDado->idadePessoa = -1;

        novoDado->tamanhoRegistro = 16 + novoDado->tamanhoNomePessoa + novoDado->tamanhoNomeUsuario;
        int offset = ftell(arqD);

        fwrite(&novoDado->removido, sizeof(char), 1, arqD);
        fwrite(&novoDado->tamanhoRegistro, sizeof(int), 1, arqD);
        fwrite(&novoDado->idPessoa, sizeof(int), 1, arqD);
        fwrite(&novoDado->idadePessoa, sizeof(int), 1, arqD);
        fwrite(&novoDado->tamanhoNomePessoa, sizeof(int), 1, arqD);
        fwrite(novoDado->nomePessoa,  sizeof(char), novoDado->tamanhoNomePessoa, arqD);
        fwrite(&novoDado->tamanhoNomeUsuario, sizeof(int), 1, arqD);
        fwrite(novoDado->nomeUsuario, sizeof(char), novoDado->tamanhoNomeUsuario, arqD);
        DadosIndice[Cabecalho->quantidadePessoas] = malloc(sizeof(RegistroIndice));
        DadosIndice[Cabecalho->quantidadePessoas]->idPessoa = novoDado->idPessoa;
        DadosIndice[Cabecalho->quantidadePessoas]->byteOffset = offset;
        Cabecalho->quantidadePessoas++;
        free(novoDado);
    }
    long long int offset = ftell(arqD);

    Cabecalho->status = '1';
    fseek(arqD, 0, SEEK_SET);
    fwrite(&Cabecalho->status, sizeof(char), 1, arqD);
    fwrite(&Cabecalho->quantidadePessoas, sizeof(int), 1, arqD);
    fseek(arqD, 9, SEEK_SET);
    fwrite(&offset, sizeof(long long int), 1 , arqD);

    fclose(arqI);

    qsort(DadosIndice, Cabecalho->quantidadePessoas, sizeof(RegistroIndice *), compararIndicePorID);
    if(!escreveIndice(arquivoIndice, DadosIndice, Cabecalho->quantidadePessoas))
    {
        return 0;
    }

    free(Cabecalho);
    for(int i = 0; i < MAXIMO; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    fclose(arqD);
    return 1;
}
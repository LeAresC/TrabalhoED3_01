#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#define MAXIMO 2000

int insereNoFinal(char *arquivoDados, char *arquivoIndice, int N)
{
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "r+b");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }
    CabecalhoPessoa *Cabecalho = leCabecalhoPessoa(arqD);

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

        scanf("%d %999[^,],", &cnt, buffer1);
        scanQuoteString(novoDado->nomePessoa);
        char aux;
        scanf("%c %999[^,],", &aux, buffer2);
        scanQuoteString(novoDado->nomeUsuario);

        novoDado->tamanhoNomePessoa = strlen(novoDado->nomePessoa);
        novoDado->tamanhoNomeUsuario = strlen(novoDado->nomeUsuario);

        novoDado->idPessoa = atoi(buffer1);
        if (strcmp(buffer2, "NULO") != 0)
            novoDado->idadePessoa = atoi(buffer2);
        else
            novoDado->idadePessoa = -1;

        novoDado->tamanhoRegistro = 16 + novoDado->tamanhoNomePessoa + novoDado->tamanhoNomeUsuario;
        long offset = ftell(arqD);

        insereFinalPessoa(arqD, novoDado, offset);
        DadosIndice[Cabecalho->quantidadePessoas] = malloc(sizeof(RegistroIndice));
        insereFinalIndice(DadosIndice, Cabecalho->quantidadePessoas, novoDado->idPessoa, offset);

        Cabecalho->quantidadePessoas++;
        Cabecalho->proxByteOffset = ftell(arqD);
        free(novoDado);
    }
    atualizaCabecalhoPessoa(arqD, Cabecalho);
    fclose(arqI);
    escreveIndice(arquivoIndice, DadosIndice, Cabecalho->quantidadePessoas);
    free(Cabecalho);
    free(DadosIndice);
    fclose(arqD);
    return 1;
}
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 500

RegistroPessoa ***buscaNDados(char *arquivoDados, char *arquivoIndice, int N)
{
    // Declara os arquivos de dados e de indice e verifica se foi a abertura ocorreu corretamente
    FILE *arqD = fopen(arquivoDados, "rb");
    FILE *arqI = fopen(arquivoIndice, "rb");
    if (arqD == NULL || arqI == NULL)
    {
        return NULL;
    }
    CabecalhoPessoa *Cabecalho = leCabecalhoPessoa(arqD);
    // Le o Arquivo de Indice todo para se fazer a busca binária
    RegistroIndice **DadosIndice = leArquivoIndice(arqI, Cabecalho->quantidadePessoas);
    RegistroPessoa ***RetornoDados = (RegistroPessoa ***)malloc(sizeof(RegistroPessoa **) * N);
    // Loop da funcionalidade que busca os Dados n vezes
    for (int i = 0; i < N; i++)
    {
        RetornoDados[i] = (RegistroPessoa **)malloc(sizeof(RegistroPessoa *) * (Cabecalho->quantidadePessoas + 1));
        int *Offsets = buscaDados(arqD, DadosIndice);
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
            {
                RetornoDados[i][j] = (RegistroPessoa*)malloc(sizeof(RegistroPessoa));
                RetornoDados[i][j]->removido = -2;
                break;
            }
            fseek(arqD, Offsets[j], SEEK_SET);
            RetornoDados[i][j] = leRegistroPessoa(arqD);
        }
    }
    free(DadosIndice);
    free(Cabecalho);
    fclose(arqD);
    fclose(arqI);
    return RetornoDados;
}
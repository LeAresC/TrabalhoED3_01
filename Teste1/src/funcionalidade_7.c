#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#define MAXIMO 1000

int atualizaArquivos(char *arquivoDados, char *arquivoIndice, int N)
{
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "r+b");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }

    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arqD);

    RegistroIndice **DadosIndice = leArquivoIndice(arqI, CabecalhoP->quantidadePessoas);

    int tamanhoIndice = CabecalhoP->quantidadePessoas;

    for (int i = 0; i < N; i++)
    {
        int cnt;
        char nomeCampo1[MAXIMO];
        char valorCampo1[MAXIMO];
        scanf("%d %999[^=]=", &cnt, nomeCampo1);
        scanQuoteString(valorCampo1);

        char nomeCampo2[MAXIMO];
        char valorCampo2[MAXIMO];
        scanf(" %999[^=]=", nomeCampo2);
        scanQuoteString(valorCampo2);

        long *Offsets = buscaDados(arqD, DadosIndice, nomeCampo1, valorCampo1);
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
                break;

            fseek(arqD, Offsets[j], SEEK_SET);
            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);

            if (strcmp(nomeCampo2, "idPessoa") == 0)
            {

            }
            else if (strcmp(nomeCampo2, "idadePessoa") == 0)
            {
            }
            else if (strcmp(nomeCampo2, "nomePessoa") == 0)
            {
                if (strlen(valorCampo2) > strlen(RegistroAtual->nomePessoa))
                {
                    removeRegistroOffset(arqD, Offsets[j], DadosIndice, tamanhoIndice);
                    strcpy(RegistroAtual->nomePessoa, valorCampo2);
                    fseek(arqD, 0, SEEK_END);
                    long offset = ftell(arqD);
                    insereFinal(arqD, RegistroAtual, DadosIndice, tamanhoIndice, offset);

                    tamanhoIndice++;
                    CabecalhoP->quantidadeRemovidos++;
                    CabecalhoP->proxByteOffset = ftell(arqD);
                }
                else
                {
                    
                }
            }
            else if (strcmp(nomeCampo2, "nomeUsuario") == 0)
            {
                if (strlen(valorCampo2) > strlen(RegistroAtual->nomeUsuario))
                {
                    removeRegistroOffset(arqD, Offsets[j], DadosIndice, tamanhoIndice);
                    strcpy(RegistroAtual->nomeUsuario, valorCampo2);
                    fseek(arqD, 0, SEEK_END);
                    long offset = ftell(arqD);
                    insereFinal(arqD, RegistroAtual, DadosIndice, tamanhoIndice, offset);

                    tamanhoIndice++;
                    CabecalhoP->quantidadeRemovidos++;
                    CabecalhoP->proxByteOffset = ftell(arqD);
                }
                else
                {

                }
            }
        }
    }
    free(CabecalhoP);
    for (int i = 0; i < tamanhoIndice; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    free(CabecalhoP);
    fclose(arqI);
    fclose(arqD);
    return 1;
}
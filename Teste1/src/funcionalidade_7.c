#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#define MAXIMO 2000
#define INF 1e9

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
        int cnt;   scanf("%d", &cnt);
        char nomeCampo1[MAXIMO];
        char valorCampo1[MAXIMO];
        char nomeCampo2[MAXIMO];
        char valorCampo2[MAXIMO];

        leInput(nomeCampo1, valorCampo1);
        leInput(nomeCampo2, valorCampo2);
        long *Offsets = buscaDados(arqD, DadosIndice, nomeCampo1, valorCampo1);

        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
                break;
            fseek(arqD, Offsets[j], SEEK_SET);
            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);
            if (strcmp(nomeCampo2, "idPessoa") == 0)
            {
                int idAnterior = RegistroAtual->idPessoa;
                RegistroAtual->idPessoa = atoi(valorCampo2);
                insereMeioPessoa(arqD, Offsets[j], RegistroAtual);
                atualizaNoIndice(DadosIndice, tamanhoIndice, RegistroAtual->idPessoa, Offsets[j], idAnterior);
            }
            else if (strcmp(nomeCampo2, "idadePessoa") == 0)
            {
                RegistroAtual->idadePessoa = atoi(valorCampo2);
                insereMeioPessoa(arqD, Offsets[j], RegistroAtual);
            }
            else if (strcmp(nomeCampo2, "nomePessoa") == 0)
            {
                strcpy(RegistroAtual->nomePessoa, valorCampo2);
                RegistroAtual->tamanhoNomePessoa = strlen(valorCampo2);

                int tamanhoNec = 16 + RegistroAtual->tamanhoNomePessoa + RegistroAtual->tamanhoNomeUsuario;

                if (tamanhoNec > RegistroAtual->tamanhoRegistro)
                {

                    removeRegistroOffsetPessoa(arqD, Offsets[j], RegistroAtual);
                    RegistroAtual->tamanhoRegistro = tamanhoNec;
                    insereFinalPessoa(arqD, RegistroAtual, CabecalhoP->proxByteOffset);
                    atualizaNoIndice(DadosIndice, tamanhoIndice, RegistroAtual->idPessoa, CabecalhoP->proxByteOffset, RegistroAtual->idPessoa);

                    CabecalhoP->quantidadeRemovidos++;
                    CabecalhoP->proxByteOffset = ftell(arqD);
                }
                else
                {
                    insereMeioPessoa(arqD, Offsets[j], RegistroAtual);
                }
            }
            else if (strcmp(nomeCampo2, "nomeUsuario") == 0)
            {
                strcpy(RegistroAtual->nomeUsuario, valorCampo2);
                RegistroAtual->tamanhoNomeUsuario = strlen(valorCampo2);

                int tamanhoNec = 16 + RegistroAtual->tamanhoNomePessoa + RegistroAtual->tamanhoNomeUsuario;

                if (tamanhoNec > RegistroAtual->tamanhoRegistro)
                {
                    removeRegistroOffsetPessoa(arqD, Offsets[j], RegistroAtual);
                    RegistroAtual->tamanhoRegistro = tamanhoNec;
                    insereFinalPessoa(arqD, RegistroAtual, CabecalhoP->proxByteOffset);
                    atualizaNoIndice(DadosIndice, tamanhoIndice, RegistroAtual->idPessoa, CabecalhoP->proxByteOffset, RegistroAtual->idPessoa);

                    CabecalhoP->quantidadeRemovidos++;
                    CabecalhoP->proxByteOffset = ftell(arqD);
                }
                else
                {
                    insereMeioPessoa(arqD, Offsets[j], RegistroAtual);
                }
            }

            free(RegistroAtual->nomePessoa);
            free(RegistroAtual->nomeUsuario);
            free(RegistroAtual);
        }
        atualizaCabecalhoPessoa(arqD, CabecalhoP);
    }
    fclose(arqI);
    escreveIndice(arquivoIndice, DadosIndice, CabecalhoP->quantidadePessoas);
    free(DadosIndice);
    free(CabecalhoP);
    fclose(arqD);
    return 1;
}
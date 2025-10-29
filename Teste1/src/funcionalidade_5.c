#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#define MAXIMO 1000
#define INF 1e9

int removeArquivoPessoa(char *arquivoDados, char *arquivoIndice, int N)
{
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "r+b");

    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }
    CabecalhoPessoa *Cabecalho = leCabecalhoPessoa(arqD);

    RegistroIndice **DadosIndice = leArquivoIndice(arqI, Cabecalho->quantidadePessoas);

    long **Offsets = (long **)malloc(sizeof(long *) * N);

    int tamanhoIndice = Cabecalho->quantidadePessoas;
    for (int i = 0; i < N; i++)
    {
        int cnt;
        char nomeCampo[MAXIMO];
        char valorCampo[MAXIMO];
        scanf("%d %999[^=]=", &cnt, nomeCampo);
        scanQuoteString(valorCampo);
        Offsets[i] = buscaDados(arqD, DadosIndice, nomeCampo, valorCampo);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[i][j] == -1)
                break;

            fseek(arqD, Offsets[i][j], SEEK_SET);

            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);

            if (RegistroAtual->removido != '1')
            {
                
                //Lida com o lixo no inicio do registro
                fseek(arqD, Offsets[i][j], SEEK_SET);
                long cnt = 0;
                char aux = '$';
                while (aux == '$')
                {
                    fread(&aux, sizeof(char), 1, arqD);
                    cnt++;
                }
                RegistroAtual->removido = '1';
                fseek(arqD, Offsets[i][j] + cnt - 1, SEEK_SET);
                fwrite(&RegistroAtual->removido, sizeof(char), 1, arqD);

                long offsetIndice = buscaBinariaIndice(DadosIndice, tamanhoIndice, RegistroAtual->idPessoa);
                DadosIndice[offsetIndice]->byteOffset = -1;
                Cabecalho->quantidadePessoas--;
                Cabecalho->quantidadeRemovidos++;

                free(RegistroAtual->nomePessoa);
                free(RegistroAtual->nomeUsuario);
            }
            free(RegistroAtual);
        }
    }
    for (int i = 0; i < tamanhoIndice; i++)
    {
        if (DadosIndice[i]->byteOffset == -1)
        {
            DadosIndice[i]->idPessoa = INF;
        }
    }

    char estabilidade = '1';
    fseek(arqD, 0, SEEK_SET);
    fwrite(&estabilidade, sizeof(char), 1, arqD);
    fwrite(&Cabecalho->quantidadePessoas, sizeof(int), 1, arqD);
    fwrite(&Cabecalho->quantidadeRemovidos, sizeof(int), 1, arqD);
    qsort(DadosIndice, tamanhoIndice, sizeof(RegistroIndice *), compararIndicePorID);

    fclose(arqI);

    if (!escreveIndice(arquivoIndice, DadosIndice, Cabecalho->quantidadePessoas))
    {
        return 0;
    }
    for (int i = 0; i < N; i++)
    {
        free(Offsets[i]);
    }
    free(Offsets);
    free(Cabecalho);
    for (int i = 0; i < MAXIMO; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    fclose(arqD);
    return 1;
}
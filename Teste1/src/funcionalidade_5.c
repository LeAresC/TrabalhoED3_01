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

    int **Offsets = (int **) malloc(sizeof(int*) * N);

    int tamanhoIndice = Cabecalho->quantidadePessoas;
    for (int i = 0; i < N; i++)
    {
      Offsets[i] = buscaDados(arqD, DadosIndice);
    }

    int IndicesRetirados[MAXIMO];
    int Contador = 0;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < MAXIMO; j++)
        {
            if(Offsets[i][j] == -1) 
            break;

            char removido = '1';
            int tamanhoRegistro;
            int idPessoa;
            fseek(arqD, Offsets[i][j], SEEK_SET);
            fwrite(&removido, sizeof(char), 1, arqD);

            fseek(arqD, Offsets[i][j] + sizeof(char), SEEK_SET);
            fread(&tamanhoRegistro, sizeof(int), 1, arqD);
            fread(&idPessoa, sizeof(int), 1 , arqD);

            long long int offsetIndice = buscaBinariaIndice(DadosIndice, tamanhoIndice, idPessoa);
            IndicesRetirados[Contador] = offsetIndice;
            Contador++;

            Cabecalho->quantidadePessoas--;
            Cabecalho->quantidadeRemovidos++;
        }
    }
    fseek(arqD, 1 , SEEK_SET);
    fwrite(&Cabecalho->quantidadePessoas, sizeof(int), 1, arqD);
    fwrite(&Cabecalho->quantidadeRemovidos, sizeof(int), 1, arqD);

    for(int i = 0; i < Contador; i++)
    {
        if(IndicesRetirados[i] >= 0)
        DadosIndice[IndicesRetirados[i]]->idPessoa = INF;
    }
    qsort(DadosIndice, tamanhoIndice, sizeof(RegistroIndice *), compararIndicePorID);

    fclose(arqI);

    if(!escreveIndice(arquivoIndice, DadosIndice, Cabecalho->quantidadePessoas))
    {
        return 0;
    }
    for(int i = 0; i < N; i++)
    {
        free(Offsets[i]);
    }
    free(Offsets);
    free(Cabecalho);
    for(int i = 0; i < MAXIMO; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    fclose(arqD);
    return 1;
}
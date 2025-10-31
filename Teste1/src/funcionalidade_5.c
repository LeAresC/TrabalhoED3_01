#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#define MAXIMO 2000
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

    for (int i = 0; i < N; i++)
    {
        int cnt;
        scanf("%d", &cnt);
        char nomeCampo[MAXIMO];
        char valorCampo[MAXIMO];
        leInput(nomeCampo, valorCampo);
        long *Offsets = buscaDados(arqD, DadosIndice, nomeCampo, valorCampo);
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
                break;
            fseek(arqD, Offsets[j], SEEK_SET);
            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);
            removeRegistroOffsetPessoa(arqD, Offsets[j], RegistroAtual);
            removeRegistroOffsetIndice(DadosIndice, Cabecalho->quantidadePessoas, RegistroAtual->idPessoa);
            Cabecalho->quantidadePessoas--;
            Cabecalho->quantidadeRemovidos++;
            free(RegistroAtual->nomePessoa);
            free(RegistroAtual->nomeUsuario);
            free(RegistroAtual);
        }
        atualizaCabecalhoPessoa(arqD, Cabecalho);
    }
    fclose(arqI);
    escreveIndice(arquivoIndice, DadosIndice, Cabecalho->quantidadePessoas);
    free(Cabecalho);
    free(DadosIndice);
    fclose(arqD);

    return 1;
}
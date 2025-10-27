#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 500
#define INF 1e8

RegistroPessoa ***buscaNDados(char *arquivoDados, char *arquivoIndice, int N)
{
    // Declara os arquivos de dados e de indice e verifica se foi a abertura ocorreu corretamente
    FILE *arqD;
    arqD = fopen(arquivoDados, "rb");
    FILE *arqI;
    arqI = fopen(arquivoIndice, "rb");
    if (arqD == NULL || arqI == NULL)
    {
        return NULL;
    }

    // Le o cabecalho do arquivo de dados
    CabecalhoPessoa *Cabecalho = leCabecalho(arqD);
    // Le o Arquivo de Indice todo para se fazer a busca binária
    RegistroIndice **DadosIndice = leArquivoIndice(arqI, Cabecalho->quantidadePessoas);

    RegistroPessoa ***RetornoDados = (RegistroPessoa ***)malloc(sizeof(RegistroPessoa **) * N);
    // Loop da funcionalidade que busca os Dados n vezes
    for (int i = 0; i < N; i++)
    {
        RetornoDados[i] = buscaDados(arqD, DadosIndice, Cabecalho);
    }
    free(DadosIndice);
    free(Cabecalho);
    fclose(arqD);
    fclose(arqI);
    return RetornoDados;
}
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 2000

int buscaNDados(char *arquivoDados, char *arquivoIndice, int N)
{
    // Declara os arquivos de dados e de indice e verifica se foi a abertura ocorreu corretamente
    FILE *arqD = fopen(arquivoDados, "rb");
    FILE *arqI = fopen(arquivoIndice, "rb");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }
    CabecalhoPessoa *Cabecalho = leCabecalhoPessoa(arqD);
    // Le o Arquivo de Indice todo para se fazer a busca binária
    RegistroIndice **DadosIndice = leArquivoIndice(arqI, Cabecalho->quantidadePessoas);
    // Loop da funcionalidade que busca os Dados n vezes
    for (int i = 0; i < N; i++)
    {
        int cnt; scanf("%d", &cnt);
        char nomeCampo[MAXIMO];
        char valorCampo[MAXIMO];
        leInput(nomeCampo, valorCampo);
        long *Offsets = buscaDados(arqD, DadosIndice, nomeCampo, valorCampo);
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
            {
                if(j == 0)
                {
                    erroRegistro();
                    printf("\n");
                }
                break;
            }
            fseek(arqD, Offsets[j], SEEK_SET);
            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);
            imprimirSaida(RegistroAtual);
        }
    }
    free(DadosIndice);
    free(Cabecalho);
    fclose(arqD);
    fclose(arqI);
    return 1;
}
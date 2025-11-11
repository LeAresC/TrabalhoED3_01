#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#define MAXIMO 2000
#define INF 1e9

int removeArquivoPessoa(char *arquivoDados, char *arquivoIndice, int N)
{
    //Abre os arquivos e verifica se conseguiu abrir
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "rb");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }

    //Le o cabecalho do arquivo pessoa
    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arqD);

    //Passa todos os dados do arquivo de Indice para a RAM
    RegistroIndice **DadosIndice = leArquivoIndice(arqI, CabecalhoP->quantidadePessoas);

    //Cria uma variável do tamanho do Indice para liberar a a memória depois
    int tamanhoMaximoIndice = CabecalhoP->quantidadePessoas;
    for (int i = 0; i < N; i++)
    {

        // Declara as variáveis e le input
        int cnt;
        scanf("%d", &cnt);
        char nomeCampo[MAXIMO];
        char valorCampo[MAXIMO];
        leInput(nomeCampo, valorCampo);


        // Declara e le os offsets que satisfazem o filtro 
        long *Offsets = buscaDados(arqD, DadosIndice, nomeCampo, valorCampo);
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
                break;
            
            // Le o registro que está no offset atual
            fseek(arqD, Offsets[j], SEEK_SET);
            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);

            //Remove dos arquivos Pessoa em Disco e indice em RAM
            removeRegistroOffsetPessoa(arqD, Offsets[j]);
            removeRegistroOffsetIndice(DadosIndice, CabecalhoP->quantidadePessoas, RegistroAtual->idPessoa);

            //Atualiza o cabecalho em RAM
            CabecalhoP->quantidadePessoas--;
            CabecalhoP->quantidadeRemovidos++;

            //Libera memória
            free(RegistroAtual->nomePessoa);
            free(RegistroAtual->nomeUsuario);
            free(RegistroAtual);

            //Atualiza o cabecalho em Disco
            atualizaCabecalhoPessoa(arqD, CabecalhoP);
        }
    }

    //Fecha os arquivos, escreve no arquivo de Indice e libera a memória
    fclose(arqI);
    escreveIndice(arquivoIndice, DadosIndice, CabecalhoP->quantidadePessoas);
    for (int i = 0; i < tamanhoMaximoIndice; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    free(CabecalhoP);
    fclose(arqD);
    return 1;
}
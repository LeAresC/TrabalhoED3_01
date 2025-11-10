#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#define MAXIMO 500
#define INF 1e8

int buscaBinariaIndice(FILE *arqI, int valorId)
{
    // Faz a busca binária no arquivo de indice
    int low = 1;
    int high = INF;
    while (low < high)
    {
        int id;
        long long int RRN;
        int mid = low + (high - low) / 2;

        // Le e armazena o ID e RRN atual correspondente
        fseek(arqI, 12 * mid, SEEK_SET);
        size_t overflow = fread(&id, sizeof(int), 1, arqI);
        fread(&RRN, sizeof(long long int), 1, arqI);

        // Se o id atual for maior que o da busca ou se está fora dos limites do arquivo high = mid
        if (overflow == 0 || id > valorId)
        {
            high = mid;
        }
        // Do contrário se o id atual for menor que o procurado low = mid + 1
        else if (id < valorId)
        {
            low = mid + 1;
        }

        // Se encontrou o ID retorna o RRN equivalente 
        else if (id == valorId)
        {
            return RRN;
        }
    }
    // Se não encontrou o ID retorna -1 (Inválido)
    return -1;
}

int buscaEmArquivo(char *arquivoDados, char *arquivoIndice, int qtd)
{
    // Declara os arquivos de dados e de indice e verifica se foi a abertura ocorreu corretamente
    FILE *arqD;
    arqD = fopen(arquivoDados, "rb");
    FILE *arqI;
    arqI = fopen(arquivoIndice, "rb");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }

    // Le o cabecalho do arquivo de dados
    CabecalhoPessoa cabecalho;
    cabecalho = leCabecalhoPessoa(arqD);

    // Loop da funcionalidade que executa n = qtd de vezes
    while (qtd--)
    {
        // Le e separa o nome campo do valor campo do input
        int cnt;
        char *nomeCampo = (char *)malloc(sizeof(char) * MAXIMO);
        char *valorCampo = (char *)malloc(sizeof(char) * MAXIMO);
        scanf("%d %[^=]", &cnt, nomeCampo);
        if (strcmp(nomeCampo, "idPessoa") == 0 || strcmp(nomeCampo, "idadePessoa") == 0)
        {
            char buffer[1];
            scanf("%1s%s", buffer, valorCampo);
        }
        else
        {
            char buffer[1];
            scanf("%1s", buffer);
            scanQuoteString(valorCampo);
        }

        // Declara o registro atual e pula o cabeçalho de arquivo de dados (17 bytes), que já foi lido
        RegistroPessoa *registroAtual;
        fseek(arqD, 17, SEEK_SET);

        //Se a procura for por ID realiza uma busca binaria para achar o RRN correspondente
        if (strcmp(nomeCampo, "idPessoa") == 0)
        {
            int ivalorCampo = atoi(valorCampo);

            //Procura por o RRN correspondente ao ID, se não encontrar retorna -1 

            int RRN = buscaBinariaIndice(arqI, ivalorCampo);
            if (RRN == -1)
            {
                erroRegistro();
                printf("\n");
            }
            // Se o RRN não for inválido busca e imprime o registro do arquivo de dados
            else
            {
                fseek(arqD, RRN, SEEK_SET);
                registroAtual = leRegistroPessoa(arqD);
                imprimirSaida(registroAtual);
            }
        }
        // Se a procura for por idade nome e usuario
        else
        {

            // Flag para se econtrar ou não um registro correspondente a chave
            int registroEncontrado = 0;

            // Busca sequencial no arquivo de dados até o seu fim
            while (cabecalho.proxByteOffset > ftell(arqD))
            {
                // Le o registro atual
                registroAtual = leRegistroPessoa(arqD);

                //Se o registro atual estiver removido pula para o próximo
                if (registroAtual->removido == '1')
                {
                    fseek(arqD, registroAtual->tamanhoRegistro, SEEK_CUR);
                }

                // Se não estiver removido verifica se a o registro atual satisfaz o filtro
                else if (strcmp(nomeCampo, "idadePessoa") == 0 && atoi(valorCampo) == registroAtual->idadePessoa)
                {
                    imprimirSaida(registroAtual);
                    registroEncontrado = 1;
                }
                else if (strcmp(nomeCampo, "nomePessoa") == 0 && strcmp(registroAtual->nomePessoa, valorCampo) == 0)
                {
                    imprimirSaida(registroAtual);
                    registroEncontrado = 1;
                }
                else if (strcmp(nomeCampo, "usuario") == 0 && strcmp(registroAtual->nomeUsuario, valorCampo) == 0)
                {
                    imprimirSaida(registroAtual);
                    registroEncontrado = 1;
                }
            }

            //Se não encontrar registro imprime o erro 
            if (!registroEncontrado)
            {
                erroRegistro();
                printf("\n");
            }
        }

        // Libera memória
        free(registroAtual->nomePessoa);
        free(registroAtual->nomeUsuario);

        free(registroAtual);
        free(nomeCampo);
        free(valorCampo);
    }

    fclose(arqD);
    fclose(arqI);
    return 1;
}

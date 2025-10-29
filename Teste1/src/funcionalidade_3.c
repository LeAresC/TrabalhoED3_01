#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 500


int lerArquivoBinario(char *nomeDoArquivo)
{
    // Abre o arquivo e verifica se conseguiu abrir corretamente
    FILE *arq;
    arq = fopen(nomeDoArquivo, "rb");
    if (arq == NULL)
    {
        return 0;
    }

    // Lê o registro de cabeçalho
    CabecalhoPessoa *cabecalho = leCabecalhoPessoa(arq);

    //Se não houver registros retorna 1
    if(cabecalho->proxByteOffset == 0)
    {
        return 1;
    }
    //Le todos os registros e imprime na saida
    while (ftell(arq) < cabecalho->proxByteOffset)
    {
        // Le o registro atual
        RegistroPessoa *registroAtual = leRegistroPessoa(arq); 
        // Se estiver removido pula para o proximo
        if (registroAtual->removido == '1')
        {
            fseek(arq, registroAtual->tamanhoRegistro, SEEK_CUR);
        }
        // Senão imprime ele por completo
        else
        {
            imprimirSaida(registroAtual);
            free(registroAtual->nomePessoa);
            free(registroAtual->nomeUsuario);
        }
        free(registroAtual);
    }
    free(cabecalho);
    fclose(arq);
    return 2;
}
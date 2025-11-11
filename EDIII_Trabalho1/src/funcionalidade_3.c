#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 2000


int lerArquivoBinario(char *nomeDoArquivo)
{
    // Abre o arqDuivo e verifica se conseguiu abrir corretamente
    FILE *arqD;
    arqD = fopen(nomeDoArquivo, "rb");
    if (arqD == NULL)
    {
        return 0;
    }

    // Lê o registro de cabeçalho
    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arqD);

    //Se não houver registros retorna 1
    if(CabecalhoP->proxByteOffset == 0)
    {
        return 1;
    }
    // Inicia um contador da quantidades de dados lidos
    int qtdDados = 0;
    //Le todos os registros e imprime na saida
    while (qtdDados < CabecalhoP->quantidadePessoas)
    {
        // Le o registro atual e descarta o lixo presente no fim do registro anterior
        descartaLixo(arqD);
        RegistroPessoa *registroAtual = leRegistroPessoa(arqD); 
        // Se estiver removido pula para o proximo
        if (registroAtual->removido == '1')
        {
            fseek(arqD, registroAtual->tamanhoRegistro, SEEK_CUR);
            continue;
        }
        // Senão imprime ele por completo
        else
        {
            imprimirSaida(registroAtual);
            free(registroAtual->nomePessoa);
            free(registroAtual->nomeUsuario);
            qtdDados++;
        }
        free(registroAtual);
    }
    free(CabecalhoP);
    fclose(arqD);
    return 2;
}
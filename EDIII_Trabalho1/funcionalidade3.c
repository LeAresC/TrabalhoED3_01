#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 500


int ler_arquivo_binario(char *nome_do_arquivo)
{
    // Abre o arquivo e verifica se conseguiu abrir corretamente
    FILE *arq;
    arq = fopen(nome_do_arquivo, "rb");
    if (arq == NULL)
    {
        return 0;
    }

    // Lê o registro de cabeçalho
    CabecalhoPessoa *cabecalho;
    cabecalho = le_cabecalho(arq);

    //Se não houver registros retorna 1
    if(cabecalho->proxByteOffset == 0)
    {
        return 1;
    }

    //Le todos os registros e imprime na saida
    while (ftell(arq) < cabecalho->proxByteOffset)
    {
        // Le o registro atual
        RegistroPessoa *registro_atual;
        registro_atual = le_registro(arq); 

        // Se estiver removido pula para o proximo
        if (registro_atual->removido == '1')
        {
            fseek(arq, registro_atual->tamanhoRegistro, SEEK_CUR);
        }
        // Senão imprime ele por completo
        else
        {
            imprimir_saida(registro_atual);
        }
        free(registro_atual->nomePessoa);
        free(registro_atual->nomeUsuario);
        free(registro_atual);
    }
    fclose(arq);
    return 2;
}
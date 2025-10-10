#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "auxiliares_busca.h"


#define MAXIMO 500

int main()
{
    int funcionalidade;
    int resultado;
    scanf("%d", &funcionalidade);
    if (funcionalidade == 1)
    {
    }

    if (funcionalidade == 2)
    {
    }

    if (funcionalidade == 3)
    {
        //Lê o nome do arquivo binário
        char arquivo_bin[MAXIMO];
        scanf("%s", arquivo_bin);

        //Returna 2 se tudo certo, 1 se o não houver registro e 0 se houver erro na abertura do arquivo
        resultado = ler_arquivo_binario(arquivo_bin);

        if(resultado == 0)
        {
            erro_abertura();
        }
        if(resultado == 1)
        {
            erro_registro();  
        }

    }
    if (funcionalidade == 4)
    {
        char arquivo_dados[MAXIMO];
        char arquivo_indice[MAXIMO];
        int qtd;
        scanf("%s", arquivo_dados);
        scanf("%s", arquivo_indice);
        scanf("%d", &qtd);

        resultado = busca_em_arquivo(arquivo_dados, arquivo_indice, qtd);
    
        if(resultado == 0)
        {
            erro_abertura();
        }
    }
}

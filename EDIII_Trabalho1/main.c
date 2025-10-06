#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcionalidades.h"

#define MAXIMO 500

int main()
{

    int funcionalidade;
    scanf("%d", &funcionalidade);
    if (funcionalidade == 1)
    {
    }

    if (funcionalidade == 2)
    {
    }

    if (funcionalidade == 3)
    {
        char *arquivo_bin = (char *)malloc(MAXIMO * sizeof(char));
        scanf("%s", arquivo_bin);
        ler_arquivo_binario(arquivo_bin);
    }
    if (funcionalidade == 4)
    {
        char *arquivo_dados = (char *)malloc(MAXIMO * sizeof(char));
        char *arquivo_indice = (char *)malloc(MAXIMO * sizeof(char));
        int qtd;
        scanf("%s", arquivo_dados);
        scanf("%s", arquivo_indice);
        scanf("%d", &qtd);
        busca_em_arquivo(arquivo_dados, arquivo_indice, qtd);
    }
}

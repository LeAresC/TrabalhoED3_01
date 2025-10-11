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
        char arquivoBin[MAXIMO];
        scanf("%s", arquivoBin);

        //Returna 2 se tudo certo, 1 se o não houver registro e 0 se houver erro na abertura do arquivo
        resultado = lerArquivoBinario(arquivoBin);

        if(resultado == 0)
        {
            erroAbertura();
        }
        if(resultado == 1)
        {
            erroRegistro();  
        }

    }
    if (funcionalidade == 4)
    {

        // Le os dados
        char arquivoDados[MAXIMO];
        char arquivoIndice[MAXIMO];
        int qtd;
        scanf("%s", arquivoDados);
        scanf("%s", arquivoIndice);
        scanf("%d", &qtd);
        
        //Retona 1 se tudo certo e 0 se não foi possível abrir o arquivo
        resultado = buscaEmArquivo(arquivoDados, arquivoIndice, qtd);
    
        if(resultado == 0)
        {
            erroAbertura();
        }
    }
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "auxiliares_busca.h"
#define MAXIMO 500
#define INF 1e8

int busca_binaria_indice(FILE *arq_i, int valor_id)
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
        fseek(arq_i, 12 * mid, SEEK_SET);
        size_t overflow = fread(&id, sizeof(int), 1, arq_i);
        fread(&RRN, sizeof(long long int), 1, arq_i);

        // Se o id atual for maior que o da busca ou se está fora dos limites do arquivo high = mid
        if (overflow == 0 || id > valor_id)
        {
            high = mid;
        }
        // Do contrário se o id atual for menor que o procurado low = mid + 1
        else if (id < valor_id)
        {
            low = mid + 1;
        }

        // Se encontrou o ID retorna o RRN equivalente 
        else if (id == valor_id)
        {
            return RRN;
        }
    }
    // Se não encontrou o ID retorna -1 (Inválido)
    return -1;
}

int busca_em_arquivo(char *arquivo_dados, char *arquivo_indice, int qtd)
{
    // Declara os arquivos de dados e de indice e verifica se foi a abertura ocorreu corretamente
    FILE *arq_d;
    arq_d = fopen(arquivo_dados, "rb");
    FILE *arq_i;
    arq_i = fopen(arquivo_indice, "rb");
    if (arq_d == NULL || arq_i == NULL)
    {
        return 0;
    }

    // Le o cabecalho do arquivo de dados
    CabecalhoPessoa *cabecalho;
    cabecalho = le_cabecalho(arq_d);

    // Loop da funcionalidade que executa n = qtd de vezes
    while (qtd--)
    {
        // Le e separa o nome campo do valor campo do input
        int cnt;
        char *nome_campo = (char *)malloc(sizeof(char) * MAXIMO);
        char *valor_campo = (char *)malloc(sizeof(char) * MAXIMO);
        scanf("%d %[^=]", &cnt, nome_campo);
        if (strcmp(nome_campo, "idPessoa") == 0 || strcmp(nome_campo, "idadePessoa") == 0)
        {
            char buffer[1];
            scanf("%1s%s", buffer, valor_campo);
        }
        else
        {
            char buffer[1];
            scanf("%1s", buffer);
            scan_quote_string(valor_campo);
        }

        // Declara o registro atual e pula o cabeçalho de arquivo de dados (17 bytes), que já foi lido
        RegistroPessoa *registro_atual;
        fseek(arq_d, 17, SEEK_SET);

        //Se a procura for por ID realiza uma busca binaria para achar o RRN correspondente
        if (strcmp(nome_campo, "idPessoa") == 0)
        {
            int ivalor_campo = atoi(valor_campo);

            //Procura por o RRN correspondente ao ID, se não encontrar retorna -1 

            int RRN = busca_binaria_indice(arq_i, ivalor_campo);
            if (RRN == -1)
            {
                erro_registro();
                printf("\n");
            }
            // Se o RRN não for inválido busca e imprime o registro do arquivo de dados
            else
            {
                fseek(arq_d, RRN, SEEK_SET);
                registro_atual = le_registro(arq_d);
                imprimir_saida(registro_atual);
            }
        }
        // Se a procura for por idade nome e usuario
        else
        {

            // Flag para se econtrar ou não um registro correspondente a chave
            int registro_encontrado = 0;

            // Busca sequencial no arquivo de dados até o seu fim
            while (cabecalho->proxByteOffset > ftell(arq_d))
            {
                // Le o registro atual
                registro_atual = le_registro(arq_d);

                //Se o registro atual estiver removido pula para o próximo
                if (registro_atual->removido == '1')
                {
                    fseek(arq_d, registro_atual->tamanhoRegistro, SEEK_CUR);
                }

                // Se não estiver removido verifica se a o registro atual satisfaz o filtro
                else if (strcmp(nome_campo, "idadePessoa") == 0 && atoi(valor_campo) == registro_atual->idadePessoa)
                {
                    imprimir_saida(registro_atual);
                    registro_encontrado = 1;
                }
                else if (strcmp(nome_campo, "nomePessoa") == 0 && strcmp(registro_atual->nomePessoa, valor_campo) == 0)
                {
                    imprimir_saida(registro_atual);
                    registro_encontrado = 1;
                }
                else if (strcmp(nome_campo, "usuario") == 0 && strcmp(registro_atual->nomeUsuario, valor_campo) == 0)
                {
                    imprimir_saida(registro_atual);
                    registro_encontrado = 1;
                }
            }

            //Se não encontrar registro imprime o erro 
            if (!registro_encontrado)
            {
                erro_registro();
                printf("\n");
            }
        }

        // Libera memória
        free(registro_atual->nomePessoa);
        free(registro_atual->nomeUsuario);

        free(registro_atual);
        free(nome_campo);
        free(valor_campo);
    }

    fclose(arq_d);
    fclose(arq_i);
    return 1;
}
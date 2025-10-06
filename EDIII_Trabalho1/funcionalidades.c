#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"
#define MAXIMO 500

void scan_quote_string(char *str)
{

    /*
     *	Use essa função para ler um campo string delimitado entre aspas (").
     *	Chame ela na hora que for ler tal campo. Por exemplo:
     *
     *	A entrada está da seguinte forma:
     *		nomeDoCampo "MARIA DA SILVA"
     *
     *	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
     *		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
     *		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
     *
     */

    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    }
    else if (R == '\"')
    {
        if (scanf("%[^\"]", str) != 1)
        { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    }
    else
    { // EOF
        strcpy(str, "");
    }
}

void imprimir_saida(Dados *registro_atual)
{
    printf("Dados da pessoa de codigo %d\n", registro_atual->id);
    printf("Nome: %s\n", registro_atual->nome);
    printf("Idade: %d\n", registro_atual->idade);
    printf("Usuario: %s\n\n", registro_atual->usuario);
}

void erro_abertura()
{
    printf("Falha no processamento do arquivo.");
    exit(0);
}

void erro_registro()
{
    printf("Registro inexistente.");
    exit(0);
}

int busca_binaria_indice(FILE *arq_i, int valor_id)
{
    int low = 1;
    int high = 10000;
    while (low < high)
    {
        int id;
        long long int RRN;
        int mid = low + (high - low) / 2;
        fseek(arq_i, 12 * mid, SEEK_SET);
        size_t overflow = fread(&id, sizeof(int), 1, arq_i);
        fread(&RRN, sizeof(long long int), 1, arq_i);
        if (overflow == 0 || id > valor_id)
        {
            high = mid;
        }
        else if (id < valor_id)
        {
            low = mid + 1;
        }
        else if (id == valor_id)
        {
            return RRN;
        }
    }
    return -1;
}
void le_informacao(Dados *registro_atual, FILE *arq)
{
    fread(&registro_atual->status, sizeof(char), 1, arq);
    fread(&registro_atual->tamanho_registro, sizeof(int), 1, arq);
    if (registro_atual->status == '1')
    {
        return;
    }
    fread(&registro_atual->id, sizeof(int), 1, arq);
    fread(&registro_atual->idade, sizeof(int), 1, arq);
    fread(&registro_atual->tamanho_nome, sizeof(int), 1, arq);
    if (registro_atual->tamanho_nome == 0)
    {
        strcpy(registro_atual->nome, "-");
        registro_atual->nome[1] = '\0';
    }
    else
    {
        fread(registro_atual->nome, sizeof(char), registro_atual->tamanho_nome, arq);
        registro_atual->nome[registro_atual->tamanho_nome] = '\0';
    }
    fread(&registro_atual->tamanho_usuario, sizeof(int), 1, arq);
    if (registro_atual->tamanho_usuario == 0)
    {
        strcpy(registro_atual->usuario, "-");
        registro_atual->usuario[1] = '\0';
    }
    else
    {
        fread(registro_atual->usuario, sizeof(char), registro_atual->tamanho_usuario, arq);
        registro_atual->usuario[registro_atual->tamanho_usuario] = '\0';
    }
}
int le_cabecalho(long long int *fim_do_arquivo, FILE *arq)
{
    fseek(arq, 9, 0);
    if (fread(fim_do_arquivo, sizeof(long long int), 1, arq) == 0 || (*fim_do_arquivo == 17))
    {
        return 1;
    }
    return 0;
}
void ler_arquivo_binario(char *nome_do_arquivo)
{

    FILE *arq;
    arq = fopen(nome_do_arquivo, "rb");
    long long int fim_do_arquivo;
    if (arq == NULL)
    {
        erro_abertura();
        return;
    }
    int registro_inexistente = le_cabecalho(&fim_do_arquivo, arq);
    if (registro_inexistente == 1)
    {
        erro_registro();
        return;
    }
    while (ftell(arq) != fim_do_arquivo)
    {

        Dados *registro_atual = (Dados *)malloc(MAXIMO * sizeof(Dados));
        le_informacao(registro_atual, arq);

        if (registro_atual->status == '1')
        {
            fseek(arq, registro_atual->tamanho_registro, SEEK_CUR);
        }
        else
        {
            imprimir_saida(registro_atual);
        }
    }
    fclose(arq);
}
void busca_em_arquivo(char *arquivo_dados, char *arquivo_indice, int qtd)
{
    FILE *arq_d;
    arq_d = fopen(arquivo_dados, "rb");
    FILE *arq_i;
    arq_i = fopen(arquivo_indice, "rb");
    if (arq_d == NULL || arq_i == NULL)
    {
        erro_abertura();
        return;
    }
    while (qtd--)
    {
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
        Dados *registro_atual = (Dados *)malloc(sizeof(Dados) * MAXIMO);
        fseek(arq_d, 0, SEEK_SET);
        if (strcmp(nome_campo, "idPessoa") == 0)
        {
            int ivalor_campo = atoi(valor_campo);
            int RRN = busca_binaria_indice(arq_i, ivalor_campo);
            if (RRN == -1)
            {
                printf("Registro Inexistente\n");
            }
            else
            {
                fseek(arq_d, RRN, SEEK_CUR);
                le_informacao(registro_atual, arq_d);
                imprimir_saida(registro_atual);
            }
        }
        else
        {
            long long int fim_do_arquivo;
            int registro_inexistente = le_cabecalho(&fim_do_arquivo, arq_d);
            if (registro_inexistente == 1)
            {
                printf("Registro Inexistente\n");
                continue;
            }
            int registro_encontrado = 0;
            while (fim_do_arquivo != ftell(arq_d))
            {
                le_informacao(registro_atual, arq_d);
                if (registro_atual->status == '1')
                {
                    fseek(arq_d, registro_atual->tamanho_registro, SEEK_CUR);
                }
                else if (strcmp(nome_campo, "idadePessoa") == 0 && atoi(valor_campo) == registro_atual->idade)
                {
                    imprimir_saida(registro_atual);
                    registro_encontrado = 1;
                    break;
                }
                else if (strcmp(nome_campo, "nomePessoa") == 0 && strcmp(registro_atual->nome, valor_campo) == 0)
                {
                    imprimir_saida(registro_atual);
                    registro_encontrado = 1;
                    break;
                }
                else if (strcmp(nome_campo, "usuario") == 0 && strcmp(registro_atual->usuario, valor_campo) == 0)
                {
                    imprimir_saida(registro_atual);
                    registro_encontrado = 1;
                    break;
                }
            }
            if (!registro_encontrado)
            {
                printf("Registro Inexistente\n");
            }
        }
    }

    fclose(arq_d);
    fclose(arq_i);
}
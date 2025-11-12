#include "utils_parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char* obterProximoCampo(char **linha) {
    //Declaração de variáveis
    char *inicio, *p, *campo;
    int tamanho;

    // Verifica se a linha está vazia
    if (**linha == '\0') {
        return NULL;
    }

    // Ponteiros para o início do campo e para percorrer a linha
    inicio = *linha;
    p = inicio;

    // Avançar até encontrar a vírgula ou o final da string
    while (*p != ',' && *p != '\0') {
        p++;
    }

    // Calcular o tamanho do campo e alocar memória para ele
    tamanho = p - inicio;
    campo = (char*) malloc(tamanho + 1);
    strncpy(campo, inicio, tamanho);
    campo[tamanho] = '\0';

    if (*p == ',') {
        *linha = p + 1; // Mais campos na linha
    } else {
        *linha = p; // Acabou a linha
    }

    return campo;
}

void scanQuoteString(char *str)
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
     *		scanQuoteString(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
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

void leCriterioBusca(char *nomeCampo, char *valorCampo) {
    int lixo;
    char buffer[2];

    // Lê o critério de busca no formato "campo=valor"
    scanf("%d %[^=]", &lixo, nomeCampo);
    if (strcmp(nomeCampo, "idPessoa") == 0 || strcmp(nomeCampo, "idadePessoa") == 0) {
        // É um int. Consome o '=' e lê o valor como string.
        scanf("%1s%s", buffer, valorCampo);
    } else {
        // É uma string. Consome o '=' e chama scanQuoteString.
        scanf("%1s", buffer);
        scanQuoteString(valorCampo);
    }
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#include "io_registro.h"
#include "utils.h"
#define MAXIMO 500

void imprimirSaida(RegistroPessoa *registroAtual)
{
    // Imprime o registroAtual segundo as especificações do trabalho
    printf("Dados da pessoa de codigo %d\n", registroAtual->idPessoa);
    printf("Nome: %s\n", registroAtual->nomePessoa);
    // Se a idade do registroAtual for -1 imprime "-" do contrário imprime a idade da memória
    if (registroAtual->idadePessoa != -1)
        printf("Idade: %d\n", registroAtual->idadePessoa);
    else
        printf("Idade: -\n");
    printf("Usuario: %s\n\n", registroAtual->nomeUsuario);
}

void erroAbertura()
{
    //Imprime o erro de abertura do arquivo
    printf("Falha no processamento do arquivo.\n");
}

void erroRegistro()
{
    //Imprime o erro de registro
    printf("Registro inexistente.\n");
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

RegistroPessoa** buscaPessoas(FILE *arquivoPessoa, RegistroIndice *registroIndice, int quantidadeIndices, char *nomeCampo, char *valorCampo, int *counter) {    
    // Declara variáveis
    RegistroPessoa** lista;
    RegistroPessoa* registro;
    int capacidade, match, valorId;
    long byteOffset;
    RegistroIndice chaveBusca;

    // Inicializa o contador de registros encontrados
    *counter = 0;
    lista = NULL;

    // Se a busca for por idPessoa, usa busca binária no índice
    if (strcmp(nomeCampo, "idPessoa") == 0) {
        
        // Constrói a chave de busca
        valorId = atoi(valorCampo);
        chaveBusca.idPessoa = valorId;

        // Realiza a busca binária no índice
        RegistroIndice *resultado = (RegistroIndice*) bsearch(&chaveBusca, registroIndice, quantidadeIndices, sizeof(RegistroIndice), compararRegistrosIndice);

        // Se não encontrar, retorna NULL
        if (resultado == NULL) {
            return NULL; 
        }

        // Se encontrar, lê o registro correspondente no arquivo de pessoas
        byteOffset = resultado->byteOffset;
        fseek(arquivoPessoa, byteOffset, SEEK_SET);
        registro = leRegistroPessoa(arquivoPessoa); 

        // Se o registro for nulo ou removido, libera e retorna NULL
        if (registro == NULL || registro->removido == '1') {
            if (registro) liberaRegistroPessoa(registro);
            return NULL;
        }

        // Caso contrário, adiciona o registro à lista e retorna
        lista = (RegistroPessoa**) malloc(sizeof(RegistroPessoa*));
        lista[0] = registro;
        *counter = 1;
        return lista;

    } else {
        // Busca sequencial para os outros campos
        fseek(arquivoPessoa, 17, SEEK_SET);
        capacidade = 0;

        // Percorre todo o arquivo de pessoas
        while ((registro = leRegistroPessoa(arquivoPessoa)) != NULL) {
            // Se estiver removido, libera e continua
            if (registro->removido == '1') {
                liberaRegistroPessoa(registro);
                continue;
            }

            // Verifica se o registro atual corresponde ao critério de busca
            match = 0;
            if (strcmp(nomeCampo, "idadePessoa") == 0) {
                if (registro->idadePessoa == atoi(valorCampo)) match = 1;
            } else if (strcmp(nomeCampo, "nomePessoa") == 0) {
                if (registro->tamanhoNomePessoa > 0 && strcmp(registro->nomePessoa, valorCampo) == 0) match = 1;
            } else if (strcmp(nomeCampo, "nomeUsuario") == 0) { 
                if (registro->tamanhoNomeUsuario > 0 && strcmp(registro->nomeUsuario, valorCampo) == 0) match = 1;
            }

            // Se o registro bate com o critério, adiciona à lista
            if (match) {
                // Verifica se precisa aumentar a capacidade da lista
                if (*counter == capacidade) {
                    // Se capacidade for 0, inicializa com 8, senão dobra
                    capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                    lista = (RegistroPessoa**) realloc(lista, capacidade * sizeof(RegistroPessoa*));
                }
                // Adiciona o registro à lista e incrementa o contador
                lista[*counter] = registro;
                (*counter)++;
            } else {
                // Se não bate, libera o registro
                liberaRegistroPessoa(registro);
            }
        }
        // Retorna a lista de registros encontrados
        return lista;
    }
}
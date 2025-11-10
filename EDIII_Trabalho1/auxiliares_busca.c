#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#include "io_registro.h"
#include "utils.h"
#define MAXIMO 500

RegistroPessoa *leRegistro(FILE *arq)
{
    // Declara o ponteiro para o registro atual com um espaço na memória igual ao tamanho do registro
    RegistroPessoa *registroAtual = (RegistroPessoa*) malloc(sizeof(RegistroPessoa));
    fread(&registroAtual->removido, sizeof(char), 1, arq);
    fread(&registroAtual->tamanhoRegistro, sizeof(int), 1, arq);

    // So o registro atual foi removido retorna o registroAtual como está
    if (registroAtual->removido == '1')
    {
        return registroAtual;
    }

    // Lê id idade e tamanhoNome
    fread(&registroAtual->idPessoa, sizeof(int), 1, arq);
    fread(&registroAtual->idadePessoa, sizeof(int), 1, arq);
    fread(&registroAtual->tamanhoNomePessoa, sizeof(int), 1, arq);

    // Aloca espaço para o nome = tamanhoNome + (1byte) do \0
    registroAtual->nomePessoa = malloc(sizeof(char) * (registroAtual->tamanhoNomePessoa + 1));


    // Se não houver nome copia "-" para o registroAtual
    if (registroAtual->tamanhoNomePessoa == 0)
    {
        strcpy(registroAtual->nomePessoa, "-");
        registroAtual->nomePessoa[1] = '\0';
    }

    //Do contrário copia o nome do arquivo para o registroAtual
    else
    {
        fread(registroAtual->nomePessoa, sizeof(char), registroAtual->tamanhoNomePessoa, arq);
        registroAtual->nomePessoa[registroAtual->tamanhoNomePessoa] = '\0';
    }
    fread(&registroAtual->tamanhoNomeUsuario, sizeof(int), 1, arq);
    
    // Aloca espaço para o nomeusuario = tamanhoNomeUsuario + (1byte) do \0
    registroAtual->nomeUsuario = malloc(sizeof(char) * (registroAtual->tamanhoNomeUsuario + 1));

     // Se não houver usuario copia "-" para o registroAtual
    if (registroAtual->tamanhoNomeUsuario == 0)
    {
        strcpy(registroAtual->nomeUsuario, "-");
        registroAtual->nomeUsuario[1] = '\0';
    }
    //Do contrário copia o nome de usuario do arquivo para o registroAtual
    else
    {
        fread(registroAtual->nomeUsuario, sizeof(char), registroAtual->tamanhoNomeUsuario, arq);
        registroAtual->nomeUsuario[registroAtual->tamanhoNomeUsuario] = '\0';
    }
    // Retorna o registro atual
    return registroAtual;
}

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

CabecalhoPessoa* leCabecalho(FILE *arq)
{
    // Le o cabeçalho do arquivo de dados
    CabecalhoPessoa *cabecalho  = (CabecalhoPessoa*)malloc(sizeof(CabecalhoPessoa));
    fread(&cabecalho->status, sizeof(char), 1 , arq);
    fread(&cabecalho->quantidadePessoas, sizeof(int), 1 ,arq);
    fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arq);
    fread(&cabecalho->proxByteOffset, sizeof(long long int), 1, arq);
    return cabecalho;
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

RegistroPessoa** buscaPessoas(FILE *arquivoPessoa, RegistroIndice *registroIndice, int quantidadeIndices,
                              CabecalhoPessoa *cabecalhoPessoa, char *nomeCampo, char *valorCampo, int *counter) {
    
    RegistroPessoa** lista;
    RegistroPessoa* registro;
    int capacidade;
    char removido;
    int tamanhoRegistro;
    int match;
    long byteOffset;
    int valorId;
    RegistroIndice chaveBusca;

    *counter = 0;
    lista = NULL;

    if (strcmp(nomeCampo, "idPessoa") == 0) {
        
        valorId = atoi(valorCampo);
        chaveBusca.idPessoa = valorId;

        RegistroIndice *resultado = (RegistroIndice*) bsearch(&chaveBusca, registroIndice, quantidadeIndices, 
                                                            sizeof(RegistroIndice), compararRegistrosIndice);

        if (resultado == NULL) {
            return NULL; 
        }

        byteOffset = resultado->byteOffset;
        
        fseek(arquivoPessoa, byteOffset, SEEK_SET);
        registro = leRegistroPessoa(arquivoPessoa); 

        if (registro == NULL || registro->removido == '1') {
            if (registro) liberaRegistroPessoa(registro);
            return NULL;
        }

        lista = (RegistroPessoa**) malloc(sizeof(RegistroPessoa*));
        lista[0] = registro;
        *counter = 1;
        return lista;

    } else {
        
        fseek(arquivoPessoa, 17, SEEK_SET);
        capacidade = 0;

        while (fread(&removido, sizeof(char), 1, arquivoPessoa) == 1) {
            
            fread(&tamanhoRegistro, sizeof(int), 1, arquivoPessoa);

            if (removido == '1') {
                fseek(arquivoPessoa, tamanhoRegistro, SEEK_CUR);
                continue;
            }
            
            registro = (RegistroPessoa*) malloc(sizeof(RegistroPessoa));
            registro->removido = removido;
            registro->tamanhoRegistro = tamanhoRegistro;

            fread(&registro->idPessoa, sizeof(int), 1, arquivoPessoa);
            fread(&registro->idadePessoa, sizeof(int), 1, arquivoPessoa);
            fread(&registro->tamanhoNomePessoa, sizeof(int), 1, arquivoPessoa);

            if (registro->tamanhoNomePessoa > 0) {
                registro->nomePessoa = (char*) malloc(registro->tamanhoNomePessoa + 1);
                fread(registro->nomePessoa, registro->tamanhoNomePessoa, 1, arquivoPessoa);
                registro->nomePessoa[registro->tamanhoNomePessoa] = '\0';
            } else {
                registro->nomePessoa = (char*) malloc(1);
                registro->nomePessoa[0] = '\0';
            }

            fread(&registro->tamanhoNomeUsuario, sizeof(int), 1, arquivoPessoa);
            if (registro->tamanhoNomeUsuario > 0) {
                registro->nomeUsuario = (char*) malloc(registro->tamanhoNomeUsuario + 1);
                fread(registro->nomeUsuario, registro->tamanhoNomeUsuario, 1, arquivoPessoa);
                registro->nomeUsuario[registro->tamanhoNomeUsuario] = '\0';
            } else {
                registro->nomeUsuario = (char*) malloc(1);
                registro->nomeUsuario[0] = '\0';
            }

            match = 0;
            if (strcmp(nomeCampo, "idadePessoa") == 0) {
                if (registro->idadePessoa == atoi(valorCampo)) match = 1;
            } else if (strcmp(nomeCampo, "nomePessoa") == 0) {
                if (registro->tamanhoNomePessoa > 0 && strcmp(registro->nomePessoa, valorCampo) == 0) match = 1;
            } else if (strcmp(nomeCampo, "nomeUsuario") == 0) { 
                if (registro->tamanhoNomeUsuario > 0 && strcmp(registro->nomeUsuario, valorCampo) == 0) match = 1;
            }

            if (match) {
                if (*counter == capacidade) {
                    capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                    lista = (RegistroPessoa**) realloc(lista, capacidade * sizeof(RegistroPessoa*));
                }
                lista[*counter] = registro;
                (*counter)++;
            } else {
                liberaRegistroPessoa(registro);
            }
        }
        return lista;
    }
}
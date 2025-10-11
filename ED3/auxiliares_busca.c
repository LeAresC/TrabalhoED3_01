#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#define MAXIMO 500

RegistroPessoa *leRegistro(FILE *arq)
{
    // Le e o status do arquivo e seu tamanho e guarda os resultados em um buffer
    char buffer1;
    int buffer2;
    fread(&buffer1, sizeof(char), 1, arq);
    fread(&buffer2, sizeof(int), 1, arq);

    // Declara o ponteiro para o registro atual com um espaço na memória igual ao tamanho do registro +
    // o status(1 byte) o tamanho(4 bytes) e os 2 \0 do nome e usuário (2 bytes) = tamanhoregistro + 7 byte 

    RegistroPessoa *registroAtual = (RegistroPessoa*) malloc(buffer2 + 7);
    registroAtual->removido = buffer1;
    registroAtual->tamanhoRegistro = buffer2;

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
    CabecalhoPessoa *cabecalho  = (CabecalhoPessoa*)malloc(17);
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
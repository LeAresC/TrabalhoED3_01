#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#define MAXIMO 500

RegistroPessoa *le_registro(FILE *arq)
{
    // Le e o status do arquivo e seu tamanho e guarda os resultados em um buffer
    char buffer1;
    int buffer2;
    fread(&buffer1, sizeof(char), 1, arq);
    fread(&buffer2, sizeof(int), 1, arq);

    // Declara o ponteiro para o registro atual com um espaço na memória igual ao tamanho do registro +
    // o status(1 byte) o tamanho(4 bytes) e os 2 \0 do nome e usuário (2 bytes) = tamanhoregistro + 7 byte 

    RegistroPessoa *registro_atual = (RegistroPessoa*) malloc(buffer2 + 7);
    registro_atual->removido = buffer1;
    registro_atual->tamanhoRegistro = buffer2;

    // So o registro atual foi removido retorna o registro_atual como está
    if (registro_atual->removido == '1')
    {
        return registro_atual;
    }

    // Lê id idade e tamanhoNome
    fread(&registro_atual->idPessoa, sizeof(int), 1, arq);
    fread(&registro_atual->idadePessoa, sizeof(int), 1, arq);
    fread(&registro_atual->tamanhoNomePessoa, sizeof(int), 1, arq);

    // Aloca espaço para o nome = tamanhoNome + (1byte) do \0
    registro_atual->nomePessoa = malloc(sizeof(char) * (registro_atual->tamanhoNomePessoa + 1));


    // Se não houver nome copia "-" para o registro_atual
    if (registro_atual->tamanhoNomePessoa == 0)
    {
        strcpy(registro_atual->nomePessoa, "-");
        registro_atual->nomePessoa[1] = '\0';
    }

    //Do contrário copia o nome do arquivo para o registro_atual
    else
    {
        fread(registro_atual->nomePessoa, sizeof(char), registro_atual->tamanhoNomePessoa, arq);
        registro_atual->nomePessoa[registro_atual->tamanhoNomePessoa] = '\0';
    }
    fread(&registro_atual->tamanhoNomeUsuario, sizeof(int), 1, arq);
    
    // Aloca espaço para o nomeusuario = tamanhoNomeUsuario + (1byte) do \0
    registro_atual->nomeUsuario = malloc(sizeof(char) * (registro_atual->tamanhoNomeUsuario + 1));

     // Se não houver usuario copia "-" para o registro_atual
    if (registro_atual->tamanhoNomeUsuario == 0)
    {
        strcpy(registro_atual->nomeUsuario, "-");
        registro_atual->nomeUsuario[1] = '\0';
    }
    //Do contrário copia o nome de usuario do arquivo para o registro_atual
    else
    {
        fread(registro_atual->nomeUsuario, sizeof(char), registro_atual->tamanhoNomeUsuario, arq);
        registro_atual->nomeUsuario[registro_atual->tamanhoNomeUsuario] = '\0';
    }
    // Retorna o registro atual
    return registro_atual;
}

void imprimir_saida(RegistroPessoa *registro_atual)
{
    // Imprime o registro_atual segundo as especificações do trabalho
    printf("Dados da pessoa de codigo %d\n", registro_atual->idPessoa);
    printf("Nome: %s\n", registro_atual->nomePessoa);
    // Se a idade do registro_atual for -1 imprime "-" do contrário imprime a idade da memória
    if (registro_atual->idadePessoa != -1)
        printf("Idade: %d\n", registro_atual->idadePessoa);
    else
        printf("Idade: -\n");
    printf("Usuario: %s\n\n", registro_atual->nomeUsuario);
}

void erro_abertura()
{
    //Imprime o erro de abertura do arquivo
    printf("Falha no processamento do arquivo.\n");
}

void erro_registro()
{
    //Imprime o erro de registro
    printf("Registro inexistente.\n");
}

CabecalhoPessoa* le_cabecalho(FILE *arq)
{
    // Le o cabeçalho do arquivo de dados
    CabecalhoPessoa *cabecalho  = (CabecalhoPessoa*)malloc(17);
    fread(&cabecalho->status, sizeof(char), 1 , arq);
    fread(&cabecalho->quantidadePessoas, sizeof(int), 1 ,arq);
    fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arq);
    fread(&cabecalho->proxByteOffset, sizeof(long long int), 1, arq);
    return cabecalho;
}

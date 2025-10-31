#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#include "utilidades.h"
#define MAXIMO 2000

void leInput(char *nomeCampo, char *valorCampo)
{
    scanf(" %999[^=]=", nomeCampo);
    if ((strcmp(nomeCampo, "nomePessoa") == 0) || (strcmp(nomeCampo, "nomeUsuario") == 0))
        scanQuoteString(valorCampo);
    else
        scanf("%s", valorCampo);

    char string_nula[] = "";
    if(strcmp(valorCampo, "NULO") == 0)
    {
        strcpy(valorCampo, string_nula);
    }
}

void descartaLixo(FILE *arqD)
{
    char aux;
    do
    {
        fread(&aux, sizeof(char), 1, arqD);
    } while (aux == '$');
    fseek(arqD, -1, SEEK_CUR);
}

RegistroPessoa *leRegistroPessoa(FILE *arq)
{
    // Declara o ponteiro para o registro atual com um espaço na memória igual ao tamanho do registro
    RegistroPessoa *registroAtual = (RegistroPessoa *)malloc(sizeof(RegistroPessoa));
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
    registroAtual->nomePessoa = malloc(sizeof(char) * (MAXIMO));

    // Se não houver nome copia "-" para o registroAtual
    if (registroAtual->tamanhoNomePessoa == 0)
    {
        strcpy(registroAtual->nomePessoa, "-");
        registroAtual->nomePessoa[1] = '\0';
    }

    // Do contrário copia o nome do arquivo para o registroAtual
    else
    {
        fread(registroAtual->nomePessoa, sizeof(char), registroAtual->tamanhoNomePessoa, arq);
        registroAtual->nomePessoa[registroAtual->tamanhoNomePessoa] = '\0';
    }
    fread(&registroAtual->tamanhoNomeUsuario, sizeof(int), 1, arq);

    // Aloca espaço para o nomeusuario = tamanhoNomeUsuario + (1byte) do \0
    registroAtual->nomeUsuario = malloc(sizeof(char) * (MAXIMO));

    // Se não houver usuario copia "-" para o registroAtual
    if (registroAtual->tamanhoNomeUsuario == 0)
    {
        strcpy(registroAtual->nomeUsuario, "-");
        registroAtual->nomeUsuario[1] = '\0';
    }
    // Do contrário copia o nome de usuario do arquivo para o registroAtual
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
    // Imprime o erro de abertura do arquivo
    printf("Falha no processamento do arquivo.\n");
}

void erroRegistro()
{
    // Imprime o erro de registro
    printf("Registro inexistente.\n");
}

CabecalhoPessoa *leCabecalhoPessoa(FILE *arq)
{
    // Le o cabeçalho do arquivo de dados
    CabecalhoPessoa *cabecalho = (CabecalhoPessoa *)malloc(sizeof(CabecalhoPessoa));
    fread(&cabecalho->status, sizeof(char), 1, arq);
    fread(&cabecalho->quantidadePessoas, sizeof(int), 1, arq);
    fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arq);
    fread(&cabecalho->proxByteOffset, sizeof(long long int), 1, arq);
    return cabecalho;
}

CabecalhoIndice *leCabecalhoIndice(FILE *arqI)
{
    CabecalhoIndice *CabecalhoI = (CabecalhoIndice *)malloc(sizeof(CabecalhoIndice));
    fread(&CabecalhoI->status, sizeof(CabecalhoI->status), 1, arqI);
    fread(CabecalhoI->lixo, sizeof(CabecalhoI->lixo), 1, arqI);
    return CabecalhoI;
}

RegistroIndice **leArquivoIndice(FILE *arqI, int N)
{
    fseek(arqI, 0, SEEK_SET);
    CabecalhoIndice *CabecalhoI = leCabecalhoIndice(arqI);
    RegistroIndice **ArquivoCompleto = (RegistroIndice **)malloc(sizeof(RegistroIndice *) * MAXIMO);
    for (int i = 0; i < N; i++)
    {

        ArquivoCompleto[i] = (RegistroIndice *)malloc(sizeof(RegistroIndice));
        fread(&ArquivoCompleto[i]->idPessoa, sizeof(int), 1, arqI);
        fread(&ArquivoCompleto[i]->byteOffset, sizeof(long long int), 1, arqI);
    }
    free(CabecalhoI);
    return ArquivoCompleto;
}

int buscaBinariaIndice(RegistroIndice **ArquivoIndice, int tamanhoVetor, int valorId)
{
    // Faz a busca binária no arquivo de indice
    int l = 0;
    int r = tamanhoVetor;
    // Faz a busca binária no arquivo de indice
    while (l < r)
    {
        int id;
        int mid = l + (r - l) / 2;
        // Le e armazena o ID e RRN atual correspondente
        id = ArquivoIndice[mid]->idPessoa;
        // Se o id atual foir maior que o procurado r = mid
        if (id > valorId)
        {
            r = mid;
        }
        // Do contrário se o id atual for menor que o procurado l = mid + 1
        else if (id < valorId)
        {
            l = mid + 1;
        }

        // Se encontrou o ID retorna o indice equivalente
        else if (id == valorId)
        {
            return mid;
        }
    }
    // Se não encontrou o ID retorna -1 (Inválido)
    return -1;
}

long *buscaDados(FILE *arqD, RegistroIndice **DadosIndice, char *nomeCampo, char *valorCampo)
{
    fseek(arqD, 0, SEEK_SET);
    CabecalhoPessoa *Cabecalho = leCabecalhoPessoa(arqD);
    // Declara a lista de Offsets encontrados e define um contador da quantidade de registros encontraodr
    long *DadosEncontrados = (long *)malloc(sizeof(long) * (MAXIMO));
    for (int i = 0; i < MAXIMO; i++)
    {
        DadosEncontrados[i] = -1;
    }
    int Contador = 0;
    // Seta o ponteiro do arquivo pro começo o cabecalho
    // Se a procura for por ID realiza uma busca binaria para achar o RRN correspondente
    if (strcmp(nomeCampo, "idPessoa") == 0)
    {
        // Procura por o offset correspondente ao ID, se não encontrar retorna -1
        int valorID = atoi(valorCampo);
        long offset = buscaBinariaIndice(DadosIndice, Cabecalho->quantidadePessoas, valorID);
        if (offset != -1)
        {
            DadosEncontrados[Contador] = DadosIndice[offset]->byteOffset;
        }
    }
    // Se a procura for por idade nome e usuario
    else
    {

        // Busca sequencial no arquivo de dados até o seu fim
        while (Cabecalho->proxByteOffset > ftell(arqD))
        {
            // Le o registro atual
            descartaLixo(arqD);
            long Byteoffset = ftell(arqD);
            RegistroPessoa *registroAtual = leRegistroPessoa(arqD);
            // Se o registro atual estiver removido pula para o próximo
            if (registroAtual->removido == '1')
            {
                fseek(arqD, registroAtual->tamanhoRegistro, SEEK_CUR);
                free(registroAtual);
                continue;
            }

            // Se não estiver removido verifica se a o registro atual satisfaz o filtro
            else if (strcmp(nomeCampo, "idadePessoa") == 0)
            {
                if (strlen(valorCampo) == 0)
                {
                    if (registroAtual->idadePessoa == -1)
                    {
                        DadosEncontrados[Contador] = Byteoffset;
                        Contador++;
                    }
                }
                else
                {
                    if (atoi(valorCampo) == registroAtual->idadePessoa)
                    {
                        DadosEncontrados[Contador] = Byteoffset;
                        Contador++;
                    }
                }
            }
            else if (strcmp(nomeCampo, "nomePessoa") == 0)
            {
                if (((strlen(valorCampo) == 0) && strcmp(registroAtual->nomePessoa, "-") == 0) || (strcmp(registroAtual->nomePessoa, valorCampo) == 0))
                {
                    DadosEncontrados[Contador] = Byteoffset;
                    Contador++;
                }
            }
            else if (strcmp(nomeCampo, "nomeUsuario") == 0)
            {
                if (((strlen(valorCampo) == 0) && strcmp(registroAtual->nomeUsuario, "-") == 0) || (strcmp(registroAtual->nomeUsuario, valorCampo) == 0))
                {
                    DadosEncontrados[Contador] = Byteoffset;
                    Contador++;
                }
            }
            // Libera memória
            free(registroAtual->nomePessoa);
            free(registroAtual->nomeUsuario);
            free(registroAtual);
        }
    }

    free(Cabecalho);
    return DadosEncontrados;
}
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#define MAXIMO 500

RegistroPessoa *leRegistro(FILE *arq)
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
    registroAtual->nomePessoa = malloc(sizeof(char) * (registroAtual->tamanhoNomePessoa + 1));

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
    registroAtual->nomeUsuario = malloc(sizeof(char) * (registroAtual->tamanhoNomeUsuario + 1));

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

CabecalhoPessoa *leCabecalho(FILE *arq)
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
    fread(&CabecalhoI->status, sizeof(char), 1, arqI);
    fread(CabecalhoI->lixo, sizeof(char) * 11, 1, arqI);
    return CabecalhoI;
}

RegistroIndice **leArquivoIndice(FILE *arqI, int qtdDados)
{

    CabecalhoIndice *CabecalhoI = leCabecalhoIndice(arqI);
    RegistroIndice **ArquivoCompleto = (RegistroIndice **)malloc(sizeof(RegistroIndice *) * (qtdDados));
    for (int i = 0; i < qtdDados; i++)
    {
        ArquivoCompleto[i] = (RegistroIndice *)malloc(sizeof(RegistroIndice));
        fread(&ArquivoCompleto[i]->idPessoa, sizeof(int), 1, arqI);
        fread(&ArquivoCompleto[i]->byteOffset, sizeof(long long), 1, arqI);
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
        long long int RRN;
        int mid = l + (r - l) / 2;
        // Le e armazena o ID e RRN atual correspondente
        id = ArquivoIndice[mid]->idPessoa;
        RRN = ArquivoIndice[mid]->byteOffset;
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

        // Se encontrou o ID retorna o RRN equivalente
        else if (id == valorId)
        {
            return RRN;
        }
    }
    // Se não encontrou o ID retorna -1 (Inválido)
    return -1;
}

RegistroPessoa **buscaDados(FILE *arqD, RegistroIndice **DadosIndice, CabecalhoPessoa *Cabecalho)
{
    // Le e separa o nome campo do valor campo do input
    int cnt;
    char *nomeCampo = (char *)malloc(sizeof(char) * MAXIMO);
    char *valorCampo = (char *)malloc(sizeof(char) * MAXIMO);
    scanf("%d %[^=]", &cnt, nomeCampo);
    if (strcmp(nomeCampo, "idPessoa") == 0 || strcmp(nomeCampo, "idadePessoa") == 0)
    {
        char buffer[1];
        scanf("%1s%s", buffer, valorCampo);
    }
    else
    {
        char buffer[1];
        scanf("%1s", buffer);
        scanQuoteString(valorCampo);
    }

    //Declara a lista de registros encontrados e define um contador da quantidade de registros encontraodr
    RegistroPessoa **DadosEncontrados = (RegistroPessoa **)malloc(sizeof(RegistroPessoa *) * (Cabecalho->quantidadePessoas + 1));
    int Contador = 0;

    //Seta o ponteiro do arquivo pro começo após o cabecalho
    fseek(arqD, 17, SEEK_SET);

    // Se a procura for por ID realiza uma busca binaria para achar o RRN correspondente
    if (strcmp(nomeCampo, "idPessoa") == 0)
    {
        // Procura por o RRN correspondente ao ID, se não encontrar retorna -1

        int RRN = buscaBinariaIndice(DadosIndice, Cabecalho->quantidadePessoas, atoi(valorCampo));
        // Se o RRN não for inválido busca e imprime o registro do arquivo de dados
        if (RRN != -1)
        {
            fseek(arqD, RRN, SEEK_SET);
            DadosEncontrados[Contador] = leRegistro(arqD);
            Contador++;
        }
    }
    // Se a procura for por idade nome e usuario
    else
    {

        // Busca sequencial no arquivo de dados até o seu fim
        while (Cabecalho->proxByteOffset > ftell(arqD))
        {
            // Le o registro atual
            RegistroPessoa *registroAtual = leRegistro(arqD);

            // Se o registro atual estiver removido pula para o próximo
            if (registroAtual->removido == '1')
            {
                fseek(arqD, registroAtual->tamanhoRegistro, SEEK_CUR);
                free(registroAtual->nomePessoa);
                free(registroAtual->nomeUsuario);
                free(registroAtual);
            }

            // Se não estiver removido verifica se a o registro atual satisfaz o filtro
            else if (strcmp(nomeCampo, "idadePessoa") == 0 && atoi(valorCampo) == registroAtual->idadePessoa)
            {
                DadosEncontrados[Contador] = registroAtual;
                Contador++;
            }
            else if (strcmp(nomeCampo, "nomePessoa") == 0 && strcmp(registroAtual->nomePessoa, valorCampo) == 0)
            {
                DadosEncontrados[Contador] = registroAtual;
                Contador++;
            }
            else if (strcmp(nomeCampo, "nomeUsuario") == 0 && strcmp(registroAtual->nomeUsuario, valorCampo) == 0)
            {
                DadosEncontrados[Contador] = registroAtual;
                Contador++;
            }
            else
            {
                // Libera memória
                free(registroAtual->nomePessoa);
                free(registroAtual->nomeUsuario);

                free(registroAtual);
            }
        }
    }

    DadosEncontrados[Contador] = (RegistroPessoa*)malloc(sizeof(RegistroPessoa));
    DadosEncontrados[Contador]->removido = -2;

    free(nomeCampo);
    free(valorCampo);
    return DadosEncontrados;
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
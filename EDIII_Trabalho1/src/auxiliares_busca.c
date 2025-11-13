#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "auxiliares_busca.h"
#include "utilidades.h"
#include "utils.h"
#include "io_registro.h"
#define MAXIMO 2000

int determinarCampoBusca(char *nomeCampo)
{
    // Modulariza o campo de busca
    if (strcmp(nomeCampo, "idPessoa") == 0)
        return 0;
    if (strcmp(nomeCampo, "idadePessoa") == 0)
        return 1;
    if (strcmp(nomeCampo, "nomePessoa") == 0)
        return 2;
    if (strcmp(nomeCampo, "nomeUsuario") == 0)
        return 3;

    return -1;
}

void leInput(char *nomeCampo, char *valorCampo)
{
    // Le input do tipo nomeCampo=valorCampo se valorCampo=NULO retorna string de tamanho 0
    scanf(" %999[^=]=", nomeCampo);
    if ((strcmp(nomeCampo, "nomePessoa") == 0) || (strcmp(nomeCampo, "nomeUsuario") == 0))
        scanQuoteString(valorCampo);
    else
        scanf("%s", valorCampo);

    char string_nula[] = "";
    if (strcmp(valorCampo, "NULO") == 0)
    {
        strcpy(valorCampo, string_nula);
    }
}

void descartaLixo(FILE *arqD)
{
    // Le o lixo que ficou no final do registro passado
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

    // Aloca espaço para o nome
    registroAtual->nomePessoa = malloc(sizeof(char) * (MAXIMO));

    // Se não houver nome copia "\0" para o registroAtual
    if (registroAtual->tamanhoNomePessoa == 0)
    {
        registroAtual->nomePessoa[0] = '\0';
    }

    // Do contrário copia o nome do arquivo para o registroAtual
    else
    {
        fread(registroAtual->nomePessoa, sizeof(char), registroAtual->tamanhoNomePessoa, arq);
        registroAtual->nomePessoa[registroAtual->tamanhoNomePessoa] = '\0';
    }
    fread(&registroAtual->tamanhoNomeUsuario, sizeof(int), 1, arq);

    // Aloca espaço para o nomeusuario
    registroAtual->nomeUsuario = malloc(sizeof(char) * (MAXIMO));

    // Se não houver usuario copia "\0" para o registroAtual
    if (registroAtual->tamanhoNomeUsuario == 0)
    {
        registroAtual->nomeUsuario[0] = '\0';
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
    if(registroAtual->tamanhoNomePessoa != 0)
    printf("Nome: %s\n", registroAtual->nomePessoa);
    else printf("Nome: -\n");
    // Se a idade do registroAtual for -1 imprime "-" do contrário imprime a idade da memória
    if (registroAtual->idadePessoa != -1)
        printf("Idade: %d\n", registroAtual->idadePessoa);
    else
        printf("Idade: -\n");
    if(registroAtual->tamanhoNomeUsuario != 0)
    printf("Usuario: %s\n\n", registroAtual->nomeUsuario);
    else printf("Usuario: -\n");
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
    // Le o cabecalho do arquivo de Indice
    CabecalhoIndice *CabecalhoI = (CabecalhoIndice *)malloc(sizeof(CabecalhoIndice));
    fread(&CabecalhoI->status, sizeof(CabecalhoI->status), 1, arqI);
    fread(CabecalhoI->lixo, sizeof(CabecalhoI->lixo), 1, arqI);
    return CabecalhoI;
}

RegistroIndice **leArquivoIndice(FILE *arqI, int N)
{

    // Le o arquivo de Indice como um todo
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
        int indice = l + (r - l) / 2;
        // Le e armazena o ID e RRN atual correspondente
        id = ArquivoIndice[indice]->idPessoa;
        // Se o id atual foir maior que o procurado r = mid
        if (id > valorId)
        {
            r = indice;
        }
        // Do contrário se o id atual for menor que o procurado l = mid + 1
        else if (id < valorId)
        {
            l = indice + 1;
        }

        // Se encontrou o ID retorna o indice equivalente
        else if (id == valorId)
        {
            return indice;
        }
    }
    // Se não encontrou o ID retorna -1 (Inválido)
    return -1;
}

// Retorna uma lista de offsets
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
    // Seta um contador para a quantidade de dados encontrados
    int Contador = 0;
    // Seta um contador para a quantidade de dados válidos(não removidos) lidos
    int qtdDados = 0;
    // Determina o campo de busca (0 = idPessoa; 1 = idadePessoa; 2 = nomePessoa; 3 = nomeUsuario)
    int campoBusca = determinarCampoBusca(nomeCampo);
    // Campo que verifica qual o offset atual;
    long OffsetAtual = 17;
    // Se a procura for por ID realiza uma busca binaria para achar o RRN correspondente
    if (campoBusca == 0)
    {
        // Procura por o offset correspondente ao ID, se não encontrar retorna -1
        int valorID = atoi(valorCampo);
        int indice = buscaBinariaIndice(DadosIndice, Cabecalho->quantidadePessoas, valorID);
        if (indice != -1)
        {
            DadosEncontrados[Contador] = DadosIndice[indice]->byteOffset;
        }
    }
    // Se a procura for por idade nome e usuario
    else
    {
        // Busca sequencial no arquivo de dados até o seu fim
        while (qtdDados < Cabecalho->quantidadePessoas)
        {
            // Descarta o lixo do final do registro anterior e posiciona o cursor corretamente
            descartaLixo(arqD);
            // Le o registro atual
            RegistroPessoa *registroAtual = leRegistroPessoa(arqD);
            // Se o registro atual estiver removido pula para o próximo
            if (registroAtual->removido == '1')
            {
                fseek(arqD, registroAtual->tamanhoRegistro, SEEK_CUR);
                OffsetAtual += registroAtual->tamanhoRegistro + 5;
                free(registroAtual);
                continue;
            }

            // Se não estiver removido verifica se a o registro atual satisfaz o filtro
            else if (campoBusca == 1)
            {
                if (strlen(valorCampo) == 0 && registroAtual->idadePessoa == -1)
                {
                    DadosEncontrados[Contador] = OffsetAtual;
                    Contador++;
                }
                else
                {
                    if (atoi(valorCampo) == registroAtual->idadePessoa)
                    {
                        DadosEncontrados[Contador] = OffsetAtual;
                        Contador++;
                    }
                }
            }
            else if (campoBusca == 2)
            {
                if (((strlen(valorCampo) == 0) && registroAtual->tamanhoNomePessoa == 0) || (strcmp(registroAtual->nomePessoa, valorCampo) == 0))
                {
                    DadosEncontrados[Contador] = OffsetAtual;
                    Contador++;
                }
            }
            else if (campoBusca == 3)
            {
                if (((strlen(valorCampo) == 0) && registroAtual->tamanhoNomeUsuario == 0) || (strcmp(registroAtual->nomeUsuario, valorCampo) == 0))
                {
                    DadosEncontrados[Contador] = OffsetAtual;
                    Contador++;
                    break;
                }
            }

            // Aumenta a quantidade de dados lidos
            qtdDados++;
            // Atualiza o OffsetAtual
            OffsetAtual += registroAtual->tamanhoRegistro + 5;
            // Libera memória
            free(registroAtual->nomePessoa);
            free(registroAtual->nomeUsuario);
            free(registroAtual);
        }
    }

    // Libera a memoria do cabecalho e retorna os Offsets encontrados
    free(Cabecalho);
    return DadosEncontrados;
}

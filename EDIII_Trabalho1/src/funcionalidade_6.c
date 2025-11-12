#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#define MAXIMO 2000

// Função que insere os dados lidos em um registro e o retorna
RegistroPessoa *preparaRegistro(char *nomePessoa, char *nomeUsuario, char *valorId, char *idadePessoa)
{
    RegistroPessoa *novoDado = (RegistroPessoa *)malloc(sizeof(RegistroPessoa));
    novoDado->removido = '0';
    novoDado->nomePessoa = (char *)malloc(sizeof(char) * MAXIMO);
    novoDado->nomeUsuario = (char *)malloc(sizeof(char) * MAXIMO);
    strcpy(novoDado->nomePessoa, nomePessoa);
    strcpy(novoDado->nomeUsuario, nomeUsuario);
    novoDado->tamanhoNomePessoa = strlen(novoDado->nomePessoa);
    novoDado->tamanhoNomeUsuario = strlen(novoDado->nomeUsuario);

    novoDado->idPessoa = atoi(valorId);
    if (strcmp(idadePessoa, "NULO") != 0)
        novoDado->idadePessoa = atoi(idadePessoa);
    else
        novoDado->idadePessoa = -1;

    novoDado->tamanhoRegistro = 16 + novoDado->tamanhoNomePessoa + novoDado->tamanhoNomeUsuario;
    return novoDado;
}

int insereNoFinal(char *arquivoDados, char *arquivoIndice, int N)
{
    // Abre os arquivos e verifica se conseguiu abrir
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "r+b");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }
    // Le o cabecalho do arquivo pessoa
    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arqD);

    // Passa todos os dados do arquivo de Indice para a RAM
    RegistroIndice **DadosIndice = leArquivoIndice(arqI, CabecalhoP->quantidadePessoas);

    // Atualiza a consistência do arquivo ('0' inconsistente)
    atualizaConsistencia(arqD, arqI);

    for (int i = 0; i < N; i++)
    {
        // Declara variáveis e LeInput
        int cnt;
        char nomePessoa[MAXIMO];
        char nomeUsuario[MAXIMO];
        char valorId[MAXIMO];
        char idadePessoa[MAXIMO];
        char aux;
        scanf("%d %999[^,],", &cnt, valorId);
        scanQuoteString(nomePessoa);
        scanf("%c %999[^,],", &aux, idadePessoa);
        scanQuoteString(nomeUsuario);

        // Prepara um registro para receber os Dados
        RegistroPessoa *novoDado = preparaRegistro(nomePessoa, nomeUsuario, valorId, idadePessoa);

        // Usa o registro para atualizar o arquivo pessoa em Disco , e indice em RAM
        inserePessoa(arqD, CabecalhoP->proxByteOffset, novoDado);
        DadosIndice[CabecalhoP->quantidadePessoas] = malloc(sizeof(RegistroIndice));
        insereFinalIndice(DadosIndice, CabecalhoP->quantidadePessoas, novoDado->idPessoa, CabecalhoP->proxByteOffset);

        // Atualiza o Cabecalho Pessoa em ram
        CabecalhoP->quantidadePessoas++;
        CabecalhoP->proxByteOffset += novoDado->tamanhoRegistro + 5;

        free(novoDado->nomePessoa);
        free(novoDado->nomeUsuario);
        free(novoDado);
        // Atualiza o cabecalho Pessoa em disco
        atualizaCabecalhoPessoa(arqD, CabecalhoP);
    }
    // Fecha arquivos, libera a memória e escreve no arquivo de Indice em Disco
    fclose(arqI);
    escreveIndice(arquivoIndice, DadosIndice, CabecalhoP->quantidadePessoas);
    for (int i = 0; i < CabecalhoP->quantidadePessoas; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    free(CabecalhoP);
    fclose(arqD);
    return 1;
}

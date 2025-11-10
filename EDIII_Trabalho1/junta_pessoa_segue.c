#include "junta_pessoa_segue.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#include "auxiliares_busca.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXIMO 500

static int compararRegistroSegue(const void *a, const void *b) {
    const RegistroSegue *regA = (const RegistroSegue *)a;
    const RegistroSegue *regB = (const RegistroSegue *)b;
    
    if (regA->idPessoaQueSegue < regB->idPessoaQueSegue) return -1;
    if (regA->idPessoaQueSegue > regB->idPessoaQueSegue) return 1;
    return 0;
}

static RegistroSegue* buscarRegistrosSegue(RegistroSegue *registroSegue, int idChave, int totalRegistros, int *segueCount) {
    RegistroSegue* listaResultados;
    int capacidade;
    RegistroSegue chaveBusca;
    RegistroSegue *primeiroMatch;
    int matchIndex;
    int i;
    
    *segueCount = 0;
    listaResultados = NULL;
    capacidade = 0;

    if (totalRegistros == 0) {
        return NULL;
    }

    chaveBusca.idPessoaQueSegue = idChave;
    primeiroMatch = (RegistroSegue*) bsearch(&chaveBusca, registroSegue, totalRegistros, 
                                            sizeof(RegistroSegue), compararRegistroSegue);

    if (primeiroMatch == NULL) {
        return NULL;
    }

    matchIndex = primeiroMatch - registroSegue;

    for (i = matchIndex; i >= 0; i--) {
        if (registroSegue[i].idPessoaQueSegue == idChave) {
            if (*segueCount == capacidade) {
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                listaResultados = (RegistroSegue*) realloc(listaResultados, capacidade * sizeof(RegistroSegue));
            }
            listaResultados[*segueCount] = registroSegue[i];
            (*segueCount)++;
        } else {
            break;
        }
    }

    for (i = matchIndex + 1; i < totalRegistros; i++) {
        if (registroSegue[i].idPessoaQueSegue == idChave) {
            if (*segueCount == capacidade) {
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                listaResultados = (RegistroSegue*) realloc(listaResultados, capacidade * sizeof(RegistroSegue));
            }
            listaResultados[*segueCount] = registroSegue[i];
            (*segueCount)++;
        } else {
            break;
        }
    }
    
    return listaResultados;
}

static void printIntComNulo(int valor) {
    if (valor == -1) {
        printf("-");
    } else {
        printf("%d", valor);
    }
}

static void imprimirSaidaPessoaJoin(RegistroPessoa *pessoa) {
    if (pessoa == NULL) {
        return;
    }

    printf("Dados da pessoa de codigo %d\n", pessoa->idPessoa);
    printf("Nome: %s\n", (pessoa->tamanhoNomePessoa > 0) ? pessoa->nomePessoa : "-");
    printf("Idade: ");
    printIntComNulo(pessoa->idadePessoa);
    printf("\n");
    printf("Usuário: %s\n", (pessoa->tamanhoNomeUsuario > 0) ? pessoa->nomeUsuario : "-");
    printf("\n");

}
static void imprimirSaidaSegueJoin(RegistroSegue *segue) {
    if (segue == NULL) {
        return;
    }

    printf("Segue a pessoa de codigo: ");
    printIntComNulo(segue->idPessoaQueESeguida);
    printf("\n");
    printf("Justificativa para seguir: ");
        switch (segue->grauAmizade) {
            case '0':
                printf("celebridade\n");
                break;
            case '1':
                printf("amiga de minha amiga\n");
                break;
            case '2':
                printf("minha amiga\n");
                break;
            default:
                printf("-\n");
                break;
        }
    printf("Começou a seguir em: %s\n", (segue->dataInicioQueSegue[0] == '$') ? "-" : segue->dataInicioQueSegue);
    printf("Parou de seguir em: %s\n", (segue->dataFimQueSegue[0] == '$') ? "-" : segue->dataFimQueSegue);
    printf("\n");  
}

int juntaPessoaSegue(const char *nomeArquivoPessoa, const char *nomeArquivoSegue, const char *nomeArquivoIndice, int n) {
    FILE *arquivoPessoa;
    FILE *arquivoSegue;
    FILE *arquivoIndice;
    CabecalhoPessoa cabecalhoPessoa;
    CabecalhoSegue cabecalhoSegue;
    int quantidadeIndices, matchCountPessoa, quantidadeSegue, matchCountSegue;
    RegistroIndice* registroIndice;
    int i, j, k, idx;
    char nomeCampo[MAXIMO];
    char valorCampo[MAXIMO];
    RegistroPessoa *pessoaAtual, **matchesPessoa;
    RegistroSegue *matchesSegue, *registroSegue;
    
    arquivoPessoa = fopen(nomeArquivoPessoa, "rb");
    arquivoSegue = fopen(nomeArquivoSegue, "rb");
    arquivoIndice = fopen(nomeArquivoIndice, "rb");
    
    if (arquivoPessoa == NULL || arquivoSegue == NULL || arquivoIndice == NULL) {
        erroAbertura();
        if (arquivoPessoa != NULL) fclose(arquivoPessoa);
        if (arquivoSegue != NULL) fclose(arquivoSegue);
        if (arquivoIndice != NULL) fclose(arquivoIndice);
        return 0;
    }
    
    cabecalhoPessoa = leCabecalhoPessoa(arquivoPessoa);
    cabecalhoSegue = leCabecalhoSegue(arquivoSegue);
    
    if (cabecalhoPessoa.status == '0' || cabecalhoSegue.status == '0') {
        erroAbertura();
        fclose(arquivoPessoa);
        fclose(arquivoSegue);
        fclose(arquivoIndice);
        return 0;
    }
    
    fseek(arquivoIndice, 12, SEEK_SET);
    fseek(arquivoSegue, 9, SEEK_SET);

    quantidadeIndices = cabecalhoPessoa.quantidadePessoas + cabecalhoPessoa.quantidadeRemovidos;
    quantidadeSegue = cabecalhoSegue.quantidadePessoas;
    
    registroIndice = malloc(quantidadeIndices * sizeof(RegistroIndice));
    registroSegue = malloc(quantidadeSegue * sizeof(RegistroSegue));
    
    for (idx = 0; idx < quantidadeIndices; idx++) {
        leRegistroIndice(arquivoIndice, &registroIndice[idx]);
    }

    for (idx = 0; idx < quantidadeSegue; idx++) {
        leRegistroSegue(arquivoSegue, &registroSegue[idx]);
    }
    
    fclose(arquivoIndice);
    fclose(arquivoSegue);
    
    for (i = 0; i < n; i++) {
        leCriterioBusca(nomeCampo, valorCampo);

        matchCountPessoa = 0;
        matchesPessoa = buscaPessoas(arquivoPessoa, registroIndice, quantidadeIndices,
                                     nomeCampo, valorCampo, &matchCountPessoa);
        
        if (matchCountPessoa == 0) {
            erroRegistro();
            continue;
        }
        
        for (j = 0; j < matchCountPessoa; j++) {
            pessoaAtual = matchesPessoa[j];
            
            imprimirSaidaPessoaJoin(pessoaAtual);
            
            matchCountSegue = 0;
            matchesSegue = buscarRegistrosSegue(registroSegue, pessoaAtual->idPessoa,
                                                quantidadeSegue, &matchCountSegue);
            
            for (k = 0; k < matchCountSegue; k++) {
                imprimirSaidaSegueJoin(&matchesSegue[k]);
            }
            
            free(matchesSegue);
        }
        
        for (j = 0; j < matchCountPessoa; j++) {
            liberaRegistroPessoa(matchesPessoa[j]);
        }
        free(matchesPessoa);
    }
    
    free(registroIndice);
    free(registroSegue);
    
    return 1;
}
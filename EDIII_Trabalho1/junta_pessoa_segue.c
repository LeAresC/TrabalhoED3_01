#include "junta_pessoa_segue.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#include "utils_parse.h"
#include "utils_print.h"
#include "utils_busca.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXIMO 500

// Função auxiliar para comparar registros de segue por idPessoaQueESeguida
static int compararPorIdSeguida(const void *a, const void *b) {
    const RegistroSegue *regA = (const RegistroSegue *)a;
    const RegistroSegue *regB = (const RegistroSegue *)b;
    
    if (regA->idPessoaQueESeguida < regB->idPessoaQueESeguida) return -1;
    if (regA->idPessoaQueESeguida > regB->idPessoaQueESeguida) return 1;
    return 0;
}

static RegistroSegue* buscarRegistrosSegue(RegistroSegue *registroSegue, int idChave, int totalRegistros, int *segueCount) {
    // Declara variáveis
    RegistroSegue* listaResultados;
    int capacidade;
    RegistroSegue chaveBusca;
    RegistroSegue *primeiroMatch;
    int matchIndex;
    int i;
    
    // Inicializa contadores
    *segueCount = 0;
    listaResultados = NULL;
    capacidade = 0;

    // Retorna NULL se não houver registros
    if (totalRegistros == 0) {
        return NULL;
    }

    // Realiza a busca binária para encontrar o primeiro registro que corresponde à chave
    chaveBusca.idPessoaQueSegue = idChave;
    primeiroMatch = (RegistroSegue*) bsearch(&chaveBusca, registroSegue, totalRegistros, sizeof(RegistroSegue), compararRegistroSegue);

    // Se não encontrar, retorna NULL
    if (primeiroMatch == NULL) {
        return NULL;
    }

    // Percorre para trás e para frente a partir do primeiro match encontrado
    matchIndex = primeiroMatch - registroSegue;
    for (i = matchIndex; i >= 0; i--) {
        // Verifica se o registro atual corresponde à chave
        if (registroSegue[i].idPessoaQueSegue == idChave) {
            // Se corresponder, adiciona à lista de resultados
            if (*segueCount == capacidade) {
                // Aumenta a capacidade do array dinamicamente
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                listaResultados = (RegistroSegue*) realloc(listaResultados, capacidade * sizeof(RegistroSegue));
            }
            // Adiciona o registro à lista de resultados e incrementa o contador
            listaResultados[*segueCount] = registroSegue[i];
            (*segueCount)++;
        } else {
            // Se não corresponder, interrompe a busca
            break;
        }
    }

    // Percorre para frente a partir do primeiro match encontrado
    for (i = matchIndex + 1; i < totalRegistros; i++) {
        // Verifica se o registro atual corresponde à chave
        if (registroSegue[i].idPessoaQueSegue == idChave) {
            // Se corresponder, adiciona à lista de resultados
            if (*segueCount == capacidade) {
                // Aumenta a capacidade do array dinamicamente
                capacidade = (capacidade == 0) ? 8 : capacidade * 2;
                listaResultados = (RegistroSegue*) realloc(listaResultados, capacidade * sizeof(RegistroSegue));
            }
            // Adiciona o registro à lista de resultados e incrementa o contador
            listaResultados[*segueCount] = registroSegue[i];
            (*segueCount)++;
        } else {
            // Se não corresponder, interrompe a busca
            break;
        }
    }
    
    // Ordena os resultados por idPessoaQueESeguida em ordem crescente
    if (listaResultados != NULL && *segueCount > 0) {
        qsort(listaResultados, *segueCount, sizeof(RegistroSegue), compararPorIdSeguida);
    }
    
    return listaResultados;
}

static void imprimirSaidaSegueJoin(RegistroSegue *segue) {
    // Verifica se o ponteiro é nulo
    if (segue == NULL) {
        return;
    }

    // Imprime os dados do segue
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
    // Declara variáveis
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

    // Abre os arquivos
    arquivoPessoa = fopen(nomeArquivoPessoa, "rb");
    arquivoSegue = fopen(nomeArquivoSegue, "rb");
    arquivoIndice = fopen(nomeArquivoIndice, "rb");

    // Verifica se os arquivos foram abertos corretamente
    if (arquivoPessoa == NULL || arquivoSegue == NULL || arquivoIndice == NULL) {
        erroAbertura();
        if (arquivoPessoa != NULL) fclose(arquivoPessoa);
        if (arquivoSegue != NULL) fclose(arquivoSegue);
        if (arquivoIndice != NULL) fclose(arquivoIndice);
        return 0;
    }
    
    // Lê os cabeçalhos dos arquivos
    cabecalhoPessoa = leCabecalhoPessoa(arquivoPessoa);
    cabecalhoSegue = leCabecalhoSegue(arquivoSegue);
    
    // Verifica o status dos arquivos
    if (cabecalhoPessoa.status == '0' || cabecalhoSegue.status == '0') {
        erroAbertura();
        fclose(arquivoPessoa);
        fclose(arquivoSegue);
        fclose(arquivoIndice);
        return 0;
    }
    
    // Move os ponteiros para o início dos registros
    fseek(arquivoIndice, 12, SEEK_SET);
    fseek(arquivoSegue, 9, SEEK_SET);

    // Identifica a quantidade de registros
    quantidadeIndices = cabecalhoPessoa.quantidadePessoas + cabecalhoPessoa.quantidadeRemovidos;
    quantidadeSegue = cabecalhoSegue.quantidadePessoas;
    
    // Armazena dinâmicamente os registros de indice e segue baseado na quantidade lida
    registroIndice = malloc(quantidadeIndices * sizeof(RegistroIndice));
    registroSegue = malloc(quantidadeSegue * sizeof(RegistroSegue));
    

    // Loop para leitura dos registros
    for (idx = 0; idx < quantidadeIndices; idx++) {
        leRegistroIndice(arquivoIndice, &registroIndice[idx]);
    }

    for (idx = 0; idx < quantidadeSegue; idx++) {
        leRegistroSegue(arquivoSegue, &registroSegue[idx]);
    }
    
    // Fecha os arquivos de índice e segue, pois não serão mais necessários
    fclose(arquivoIndice);
    fclose(arquivoSegue);
    
    // Loop para processar os critérios de busca
    for (i = 0; i < n; i++) {
        // Lê o critério de busca
        leCriterioBusca(nomeCampo, valorCampo);

        // Busca as pessoas que atendem ao critério
        matchCountPessoa = 0;
        matchesPessoa = buscaPessoas(arquivoPessoa, registroIndice, quantidadeIndices, nomeCampo, valorCampo, &matchCountPessoa);
        
        // Se não encontrar nenhum registro, imprime erro e continua para o próximo critério
        if (matchCountPessoa == 0) {
            erroRegistro();
            continue;
        }
        
        // Para cada pessoa encontrada, imprime seus dados e os registros de segue associados
        for (j = 0; j < matchCountPessoa; j++) {
            // Pega a pessoa atual
            pessoaAtual = matchesPessoa[j];
            // Imprime os dados da pessoa usando a função centralizada
            imprimirSaida(pessoaAtual);
            // Busca os registros de segue associados à pessoa atual
            matchCountSegue = 0;
            matchesSegue = buscarRegistrosSegue(registroSegue, pessoaAtual->idPessoa, quantidadeSegue, &matchCountSegue);
            // Imprime os registros de segue encontrados
            for (k = 0; k < matchCountSegue; k++) {
                // Imprime os dados do registro de segue
                imprimirSaidaSegueJoin(&matchesSegue[k]);
            }
            printf("\n");
            
            // Libera a memória alocada para os registros de segue encontrados
            free(matchesSegue);
        }
        // Libera a memória alocada para os registros de pessoa encontrados
        for (j = 0; j < matchCountPessoa; j++) {
            liberaRegistroPessoa(matchesPessoa[j]);
        }
        // Libera a memória alocada para o array de pessoas
        free(matchesPessoa);
    }
    
    // Libera memória e fecha arquivo
    fclose(arquivoPessoa);
    free(registroIndice);
    free(registroSegue);
    
    return 1;
}
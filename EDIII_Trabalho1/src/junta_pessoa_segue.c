#include "junta_pessoa_segue.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#include "auxiliares_busca.h"
#include "utilidades.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXIMO 500

static RegistroPessoa *leregistroPessoa(FILE *arq)
{
    // Le e o status do arquivo e seu tamanho e guarda os resultados em um buffer
    char buffer1;
    int buffer2;
    size_t result;
    result = fread(&buffer1, sizeof(char), 1, arq);
    if (result == 0) {
        return NULL;
    }
    result = fread(&buffer2, sizeof(int), 1, arq);

    // Declara o ponteiro para o registro atual com um espaço na memória igual ao tamanho do registro
    RegistroPessoa *registroAtual = (RegistroPessoa*) malloc(sizeof(RegistroPessoa));
    registroAtual->removido = buffer1;
    registroAtual->tamanhoRegistro = buffer2;

    // So o registro atual foi removido, pula o resto do registro e retorna
    if (registroAtual->removido == '1')
    {
        // Pula o restante do registro (tamanhoRegistro - 5 bytes já lidos: 1 char + 1 int)
        fseek(arq, registroAtual->tamanhoRegistro - 5, SEEK_CUR);
        return registroAtual;
    }

    // Lê id idade e tamanhoNome
    fread(&registroAtual->idPessoa, sizeof(int), 1, arq);
    fread(&registroAtual->idadePessoa, sizeof(int), 1, arq);
    fread(&registroAtual->tamanhoNomePessoa, sizeof(int), 1, arq);

    if (registroAtual->tamanhoNomePessoa > 0) {
        // Aloca espaço para o nome = tamanhoNome + (1byte) do \0
        registroAtual->nomePessoa = malloc((registroAtual->tamanhoNomePessoa + 1));
        fread(registroAtual->nomePessoa, registroAtual->tamanhoNomePessoa, 1, arq);
        registroAtual->nomePessoa[registroAtual->tamanhoNomePessoa] = '\0';
    } else {
        // Aloca espaço mínimo para o nome
        registroAtual->nomePessoa = malloc(1);
        registroAtual->nomePessoa[0] = '\0';
    }

    fread(&registroAtual->tamanhoNomeUsuario, sizeof(int), 1, arq);
    if(registroAtual->tamanhoNomeUsuario > 0) {
        registroAtual->nomeUsuario = malloc((registroAtual->tamanhoNomeUsuario + 1));
        fread(registroAtual->nomeUsuario, registroAtual->tamanhoNomeUsuario, 1, arq);
        registroAtual->nomeUsuario[registroAtual->tamanhoNomeUsuario] = '\0';
    } else {
        // Aloca espaço mínimo para o nome de usuário
        registroAtual->nomeUsuario = malloc(1);
        registroAtual->nomeUsuario[0] = '\0';
    }
    // Retorna o registro atual
    return registroAtual;
}



static RegistroPessoa** buscaPessoas(FILE *arquivoPessoa, RegistroIndice *registroIndice, int quantidadeIndices, char *nomeCampo, char *valorCampo, int *counter) {    
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
        registro = leregistroPessoa(arquivoPessoa); 

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
        while ((registro = leregistroPessoa(arquivoPessoa)) != NULL) {
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

static void leCriterioBusca(char *nomeCampo, char *valorCampo) {
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

// Função auxiliar para imprimir inteiro com tratamento de nulo
static void printIntComNulo(int valor) {
    if (valor == -1) {
        printf("-");
    } else {
        printf("%d", valor);
    }
}

// Função auxiliar para comparar registros de segue por idPessoaQueESeguida
static int compararPorIdSeguida(const void *a, const void *b) {
    const RegistroSegue *regA = (const RegistroSegue *)a;
    const RegistroSegue *regB = (const RegistroSegue *)b;
    
    if (regA->idPessoaQueESeguida < regB->idPessoaQueESeguida) return -1;
    if (regA->idPessoaQueESeguida > regB->idPessoaQueESeguida) return 1;
    return 0;
}

static int compararRegistroSegue(const void *a, const void *b) {
    const RegistroSegue *regA = (const RegistroSegue *)a;
    const RegistroSegue *regB = (const RegistroSegue *)b;
    
    if (regA->idPessoaQueSegue < regB->idPessoaQueSegue) return -1;
    if (regA->idPessoaQueSegue > regB->idPessoaQueSegue) return 1;
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
    CabecalhoPessoa *cabecalhoPessoa;
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
    if (cabecalhoPessoa->status == '0' || cabecalhoSegue.status == '0') {
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
    quantidadeIndices = cabecalhoPessoa->quantidadePessoas + cabecalhoPessoa->quantidadeRemovidos;
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
    free(cabecalhoPessoa);
    free(registroIndice);
    free(registroSegue);
    
    return 1;
}
#include "utils_busca.h"
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "io_registro.h"

// Função para comparar dois registros de índice pelo campo idPessoa
int compararRegistrosIndice(const void *a, const void *b) {
    // Cast dos ponteiros para o tipo correto
    const RegistroIndice *regA = (const RegistroIndice *)a;
    const RegistroIndice *regB = (const RegistroIndice *)b;
    
    // Comparação simples dos campos idPessoa
    if (regA->idPessoa < regB->idPessoa) return -1;
    if (regA->idPessoa > regB->idPessoa) return 1;
    return 0;
}

// Função para comparar dois registros de segue pelo campo idPessoaQueSegue
int compararRegistroSegue(const void *a, const void *b) {
    // Cast dos ponteiros para o tipo correto
    const RegistroSegue *regA = (const RegistroSegue *)a;
    const RegistroSegue *regB = (const RegistroSegue *)b;
    
    // Comparar por idPessoaQueSegue
    if (regA->idPessoaQueSegue < regB->idPessoaQueSegue) return -1;
    if (regA->idPessoaQueSegue > regB->idPessoaQueSegue) return 1;
    return 0;
}

// Função de busca para registros de pessoa
RegistroPessoa** buscaPessoas(FILE *arquivoPessoa, RegistroIndice *registroIndice, int quantidadeIndices, char *nomeCampo, char *valorCampo, int *counter) {    
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
        registro = leRegistroPessoa(arquivoPessoa); 

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
        while ((registro = leRegistroPessoa(arquivoPessoa)) != NULL) {
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

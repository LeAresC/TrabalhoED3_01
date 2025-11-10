#include <stdlib.h>
#include <string.h>
#include "io_csv.h"
#include "utils.h"

#define CSV_LINHA_MAX 500

// Lê um registro de pessoa do CSV
RegistroPessoa* leRegistroPessoaCsv(FILE *arquivoCsv) {
    // Declaração de variáveis
    char linha[CSV_LINHA_MAX], *campo;
    char *linha_ptr;
    RegistroPessoa *registro;

    // Lê uma linha do arquivo CSV, se não houver mais linhas, retorna NULL
    if (fgets(linha, sizeof(linha), arquivoCsv) == NULL) {
        return NULL;
    }

    // Remover o caractere de nova linha, se presente
    linha[strcspn(linha, "\r\n")] = 0;

    // Aloca memória para o registro
    registro = (RegistroPessoa*) malloc(sizeof(RegistroPessoa));
    if (registro == NULL) {
        return NULL; // Falha na alocação de memória
    }

    // Usar um ponteiro para permitir que obterProximoCampo atualize a posição
    linha_ptr = linha;

    // Campo idPessoa, garantido que não é nulo
    campo = obterProximoCampo(&linha_ptr);
    registro->idPessoa = atoi(campo);
    free(campo);

    // Campo nomePessoa, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio
        registro->nomePessoa = malloc(1);
        registro->nomePessoa[0] = '\0';
        registro->tamanhoNomePessoa = 0;
    } else {
        // Campo não nulo
        registro->nomePessoa = malloc(strlen(campo) + 1);
        strcpy(registro->nomePessoa, campo);
        registro->tamanhoNomePessoa = strlen(registro->nomePessoa);
    }
    free(campo);
    
    // Campo idadePessoa, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo, definir idadePessoa como -1
        registro->idadePessoa = -1;
    } else {
        // Campo não nulo
        registro->idadePessoa = atoi(campo);
    }
    free(campo);

    // Campo nomeUsuario, garantido que não é nulo
    campo = obterProximoCampo(&linha_ptr);
    registro->nomeUsuario = malloc(strlen(campo) + 1);
    strcpy(registro->nomeUsuario, campo);
    registro->tamanhoNomeUsuario = strlen(registro->nomeUsuario);
    free(campo);

    // Inicializar os campos restantes
    registro->removido = '0';
    registro->tamanhoRegistro = 0;

    return registro;
}

// Lê um registro de segue do CSV
RegistroSegue* leRegistroSegueCsv(FILE *arquivoCsv) {
    // Declaração de variáveis
    char linha[CSV_LINHA_MAX], *campo;
    char *linha_ptr;
    RegistroSegue *registro;

    // Lê uma linha do arquivo CSV, se não houver mais linhas, retorna NULL
    if (fgets(linha, sizeof(linha), arquivoCsv) == NULL) {
        return NULL;
    }

    // Remover o caractere de nova linha, se presente
    linha[strcspn(linha, "\r\n")] = 0;

    // Aloca memória para o registro
    registro = (RegistroSegue*) malloc(sizeof(RegistroSegue));
    if (registro == NULL) {
        return NULL; // Falha na alocação de memória
    }

    // Usar um ponteiro para permitir que obterProximoCampo atualize a posição
    linha_ptr = linha;

    // Campo idPessoaQueSegue, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo, definir idPessoaQueSegue como -1
        registro->idPessoaQueSegue = -1;
    } else {
        // Campo não nulo
        registro->idPessoaQueSegue = atoi(campo);
    }
    free(campo);

    // Campo idPessoaQueESeguida, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo, definir idPessoaQueESeguida como -1
        registro->idPessoaQueESeguida = -1;
    } else {
        // Campo não nulo
        registro->idPessoaQueESeguida = atoi(campo);
    }
    free(campo);

    // Campo dataInicioQueSegue, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio - campo fixo, preencher com '$'
        memset(registro->dataInicioQueSegue, '$', 10);
        registro->dataInicioQueSegue[10] = '\0';
    } else {
        // Campo não nulo - campo fixo, copia o conteúdo
        strncpy(registro->dataInicioQueSegue, campo, 10);
        registro->dataInicioQueSegue[10] = '\0';
    }
    free(campo);

    // Campo dataFimQueSegue, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio - campo fixo, preencher com '$'
        memset(registro->dataFimQueSegue, '$', 10);
        registro->dataFimQueSegue[10] = '\0';
    } else {
        // Campo não nulo - campo fixo, copia o conteúdo
        strncpy(registro->dataFimQueSegue, campo, 10);
        registro->dataFimQueSegue[10] = '\0';
    }
    free(campo);

    // Campo grauAmizade, pode ser nulo
    campo = obterProximoCampo(&linha_ptr);
    if (campo == NULL || strlen(campo) == 0) {
        // Campo nulo ou vazio - campo fixo, preencher com '$'
        registro->grauAmizade = '$';
    } else {
        // Campo não nulo - campo fixo, copia o primeiro caractere
        registro->grauAmizade = campo[0];
    }
    free(campo);

    // Inicializar o campo restante
    registro->removido = '1';

    return registro;
}

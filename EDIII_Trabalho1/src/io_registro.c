#include <stdlib.h>
#include <string.h>
#include "io_registro.h"
#include "data_structs.h"

// Escreve um registro de pessoa no arquivo binário
void escreveRegistroPessoa(FILE *arquivo, RegistroPessoa *registro) {
    fwrite(&registro->removido, sizeof(registro->removido), 1, arquivo);
    fwrite(&registro->tamanhoRegistro, sizeof(registro->tamanhoRegistro), 1, arquivo);
    fwrite(&registro->idPessoa, sizeof(registro->idPessoa), 1, arquivo);
    fwrite(&registro->idadePessoa, sizeof(registro->idadePessoa), 1, arquivo);
    fwrite(&registro->tamanhoNomePessoa, sizeof(registro->tamanhoNomePessoa), 1, arquivo);
    
    // Escreve o nomePessoa se não for nulo
    if (registro->tamanhoNomePessoa > 0) {
        fwrite(registro->nomePessoa, registro->tamanhoNomePessoa, 1, arquivo);
    }
    
    fwrite(&registro->tamanhoNomeUsuario, sizeof(registro->tamanhoNomeUsuario), 1, arquivo);
    fwrite(registro->nomeUsuario, registro->tamanhoNomeUsuario, 1, arquivo);
}

// Escreve um registro de segue no arquivo binário
void escreveRegistroSegue(FILE *arquivo, RegistroSegue *registro) {
    fwrite(&registro->removido, sizeof(registro->removido), 1, arquivo);
    fwrite(&registro->idPessoaQueSegue, sizeof(registro->idPessoaQueSegue), 1, arquivo);
    fwrite(&registro->idPessoaQueESeguida, sizeof(registro->idPessoaQueESeguida), 1, arquivo);
    fwrite(registro->dataInicioQueSegue, 10, 1, arquivo);  // Escreve exatamente 10 bytes
    fwrite(registro->dataFimQueSegue, 10, 1, arquivo);     // Escreve exatamente 10 bytes
    fwrite(&registro->grauAmizade, sizeof(registro->grauAmizade), 1, arquivo);
}

// Escreve um registro de índice no arquivo binário
void escreveRegistroIndice(FILE *arquivo, RegistroIndice *registro) {
    fwrite(&registro->idPessoa, sizeof(registro->idPessoa), 1, arquivo);
    fwrite(&registro->byteOffset, sizeof(registro->byteOffset), 1, arquivo);
}
// Lê um registro de segue do arquivo binário
void leRegistroSegue(FILE *arquivo, RegistroSegue *registro) {
    fread(&registro->removido, sizeof(registro->removido), 1, arquivo);
    fread(&registro->idPessoaQueSegue, sizeof(registro->idPessoaQueSegue), 1, arquivo);
    fread(&registro->idPessoaQueESeguida, sizeof(registro->idPessoaQueESeguida), 1, arquivo);
    fread(registro->dataInicioQueSegue, 10, 1, arquivo);  // Lê exatamente 10 bytes
    registro->dataInicioQueSegue[10] = '\0';              // Adiciona terminador nulo
    fread(registro->dataFimQueSegue, 10, 1, arquivo);     // Lê exatamente 10 bytes
    registro->dataFimQueSegue[10] = '\0';                 // Adiciona terminador nulo
    fread(&registro->grauAmizade, sizeof(registro->grauAmizade), 1, arquivo);
}

// Lê um registro de índice do arquivo binário
void leRegistroIndice(FILE *arquivo, RegistroIndice *registro) {
    fread(&registro->idPessoa, sizeof(registro->idPessoa), 1, arquivo);
    fread(&registro->byteOffset, sizeof(registro->byteOffset), 1, arquivo);
}

// Libera a memória de um registro de pessoa
void liberaRegistroPessoa(RegistroPessoa *registro) {
    if (registro != NULL) {
        if (registro->nomePessoa != NULL) {
            free(registro->nomePessoa);
        }
        if (registro->nomeUsuario != NULL) {
            free(registro->nomeUsuario);
        }
        free(registro);
    }
}

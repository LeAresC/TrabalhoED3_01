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

RegistroPessoa *leRegistroPessoa(FILE *arq)
{
    // Le e o status do arquivo e seu tamanho e guarda os resultados em um buffer
    char buffer1;
    int buffer2;
    fread(&buffer1, sizeof(char), 1, arq);
    fread(&buffer2, sizeof(int), 1, arq);

    // Declara o ponteiro para o registro atual com um espaço na memória igual ao tamanho do registro
    RegistroPessoa *registroAtual = (RegistroPessoa*) malloc(sizeof(RegistroPessoa));
    registroAtual->removido = buffer1;
    registroAtual->tamanhoRegistro = buffer2;

    // So o registro atual foi removido retorna o registroAtual como está
    if (registroAtual->removido == '1')
    {
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

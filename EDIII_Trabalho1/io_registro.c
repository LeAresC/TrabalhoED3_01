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
    fwrite(&registro->dataInicioQueSegue, sizeof(registro->dataInicioQueSegue), 1, arquivo);
    fwrite(&registro->dataFimQueSegue, sizeof(registro->dataFimQueSegue), 1, arquivo);
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

    // Aloca espaço para o nome = tamanhoNome + (1byte) do \0
    registroAtual->nomePessoa = malloc(sizeof(char) * (registroAtual->tamanhoNomePessoa + 1));


    // Se não houver nome copia "-" para o registroAtual
    if (registroAtual->tamanhoNomePessoa == 0)
    {
        strcpy(registroAtual->nomePessoa, "-");
        registroAtual->nomePessoa[1] = '\0';
    }

    //Do contrário copia o nome do arquivo para o registroAtual
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
    //Do contrário copia o nome de usuario do arquivo para o registroAtual
    else
    {
        fread(registroAtual->nomeUsuario, sizeof(char), registroAtual->tamanhoNomeUsuario, arq);
        registroAtual->nomeUsuario[registroAtual->tamanhoNomeUsuario] = '\0';
    }
    // Retorna o registro atual
    return registroAtual;
}

// Lê um registro de segue do arquivo binário
void leRegistroSegue(FILE *arquivo, RegistroSegue *registro) {
    fread(&registro->removido, sizeof(registro->removido), 1, arquivo);
    fread(&registro->idPessoaQueSegue, sizeof(registro->idPessoaQueSegue), 1, arquivo);
    fread(&registro->idPessoaQueESeguida, sizeof(registro->idPessoaQueESeguida), 1, arquivo);
    fread(&registro->dataInicioQueSegue, sizeof(registro->dataInicioQueSegue), 1, arquivo);
    fread(&registro->dataFimQueSegue, sizeof(registro->dataFimQueSegue), 1, arquivo);
    fread(&registro->grauAmizade, sizeof(registro->grauAmizade), 1, arquivo);
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

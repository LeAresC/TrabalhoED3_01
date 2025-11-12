#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils_print.h"
#include "data_structs.h"

void printIntComNulo(int valor) {
	// Se o valor for -1, imprime "-", caso contrário imprime o valor
    if (valor == -1) {
        printf("-");
    } else {
        printf("%d", valor);
    }
}

void imprimirSaida(RegistroPessoa *registroAtual)
{
    // Verifica se o ponteiro é nulo
    if (registroAtual == NULL) {
        return;
    }

    // Imprime os dados da pessoa segundo as especificações do trabalho
    printf("Dados da pessoa de codigo %d\n", registroAtual->idPessoa);
    printf("Nome: %s\n", (registroAtual->tamanhoNomePessoa > 0) ? registroAtual->nomePessoa : "-");
    printf("Idade: ");
    printIntComNulo(registroAtual->idadePessoa);
    printf("\n");
    printf("Usuario: %s\n", (registroAtual->tamanhoNomeUsuario > 0) ? registroAtual->nomeUsuario : "-");
    printf("\n");
}

void erroAbertura()
{
    //Imprime o erro de abertura do arquivo
    printf("Falha no processamento do arquivo.\n");
}

void erroRegistro()
{
    //Imprime o erro de registro
    printf("Registro inexistente.\n\n");
}

/*
Função para imprimir dados salvos no arquivo em binário
(util para comparar saida no run codes)
*/
void binarioNaTela(char *nomeArquivoBinario) { 

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}
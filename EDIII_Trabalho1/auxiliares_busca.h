#include <stdio.h>
#include "data_structs.h"

void erro_abertura();
void erro_registro();
void imprimir_saida(RegistroPessoa *registro_atual);
int ler_arquivo_binario(char *nome_do_arquivo);
int busca_em_arquivo(char *arquivo_dados, char *arquivo_indice,int qtd);
RegistroPessoa *le_registro(FILE *arq);
int busca_binaria_indice(FILE *arq_i, int valor_campo);
CabecalhoPessoa *le_cabecalho(FILE *arq);
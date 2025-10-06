typedef struct indice{
    int id;
    long long int RRN;
}Indice;

typedef struct dados{
    char status;
    int tamanho_registro;
    int id;
    int idade;
    int tamanho_nome;
    int tamanho_usuario;
    char nome[500];
    char usuario[500];
}Dados;

void erro_abertura();
void erro_registro();
void imprimir_saida(Dados *registro_atual);
void ler_arquivo_binario(char *nome_do_arquivo);
void busca_em_arquivo(char *arquivo_dados, char *arquivo_indice,int qtd);
int busca_binaria_indice(FILE *arq_i, int valor_campo);
void imprimeListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas);
void InsereAdjacencia(FILE *arqP, FILE *arqS, int *Map, Lista *ListaDeAdjacencia, int qtdSegues, int qtdPessoas, RegistroIndice *DadosIndice);
Lista *criaListaAdjacencia(int Tipo, FILE *arqP, FILE *arqS, RegistroIndice *DadosIndice, int qtdPessoas, int qtdSegues);
int compararDatas(char *d1, char *d2);
void trocaConteudo(No *no1, No *no2);
void ordenarListaInterna(Lista ListaAtual);
int compararListas(const void *a, const void *b);
void ordenarListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas);
void copiaNoOffset(FILE *arqP, char *nomeUsuario);
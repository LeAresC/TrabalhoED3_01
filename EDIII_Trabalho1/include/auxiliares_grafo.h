void imprimeListaAdjacencia(Adjacentes *ListaAdjacencia, int qtdPessoas);
int OrdenaAdjacencia(const void *a, const void *b);
void InsereAdjacencia(FILE *arqP, FILE *arqS, int Map[], Adjacentes *listaAdjacencia, int qtdSegues, int qtdPessoas, RegistroIndice **DadosIndice);
Adjacentes *criaListaAdjacencia(FILE *arqP, FILE *arqS, RegistroIndice **DadosIndice, int qtdPessoas, int qtdSegues);
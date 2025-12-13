#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#define MAXIMO 5000

// Variável global que determina o tipo de Grafo (0 = normal , 1 = Transposto)
int TipoGrafo = 0;

// Faz a impressão da lista de Ajacencia
void imprimeListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    for (int i = 0; i < qtdPessoas; i++)
    {
        // Inicializa no No atual
        int qtdSegue = ListaAdjacencia[i].tamanho;
        No *at = ListaAdjacencia[i].inicio;
        for (int j = 0; j < qtdSegue; j++)
        {
            // Se for um grafo normal imprime Segue->QueESeguida se transposto imprime QueESeguida->Segue
            if (TipoGrafo == 0)
            {
                printf("%s, ", at->nomeUsuarioQueSegue);
                printf("%s, ", at->nomeUsuarioQueESeguida);
            }
            else
            {
                printf("%s, ", at->nomeUsuarioQueESeguida);
                printf("%s, ", at->nomeUsuarioQueSegue);
            }
            // Imprime o Conteudo lidando com o Nulo
            if (strlen(at->dataInicioQueSegue) != 0)
                printf("%s, ", at->dataInicioQueSegue);
            else
                printf("NULO, ");
            if (strlen(at->dataFimQueSegue) != 0)
                printf("%s, ", at->dataFimQueSegue);
            else
                printf("NULO, ");
            if (at->grauAmizade != '$')
                printf("%c", at->grauAmizade);
            else
                printf("NULO");
            printf("\n");
            at = at->prox;
        }
        // Não printa o espaço se a lista atual está vazia
        if (ListaAdjacencia[i].tamanho != 0)
            printf("\n");
    }
}

// Faz uma pequena Leitura no Arquivo Pessoa para encontrar o nomeUsuario
void copiaNoOffset(FILE *arqP, char *NomeUsuario)
{
    RegistroPessoa registroAtual;

    // Verifica se a leitura do cabeçalho funcionou
    if (fread(&registroAtual.removido, sizeof(char), 1, arqP) != 1)
    {
        registroAtual.removido = 'X'; // Marca erro
    }
    // Le o restante do registro
    fread(&registroAtual.tamanhoRegistro, sizeof(int), 1, arqP);

    fread(&registroAtual.idPessoa, sizeof(int), 1, arqP);
    fread(&registroAtual.idadePessoa, sizeof(int), 1, arqP);

    fread(&registroAtual.tamanhoNomePessoa, sizeof(int), 1, arqP);
    char temp[100];
    if (registroAtual.tamanhoNomePessoa > 0)
    {
        fread(temp, sizeof(char), registroAtual.tamanhoNomePessoa, arqP);
    }
    fread(&registroAtual.tamanhoNomeUsuario, sizeof(int), 1, arqP);

    // Le o nomeUsuario e guarda na String desejada (nomeUsuarioSegue ou nomeUsuarioQueESeguido)
    if (registroAtual.tamanhoNomeUsuario > 0)
    {
        fread(NomeUsuario, sizeof(char), registroAtual.tamanhoNomeUsuario, arqP);
    }
    NomeUsuario[registroAtual.tamanhoNomeUsuario] = '\0';
}

void InsereAdjacencia(FILE *arqP, FILE *arqS, int *Map, Lista *ListaDeAdjacencia, int qtdSegues, int qtdPessoas, RegistroIndice *DadosIndice)
{
    fseek(arqS, 9, SEEK_SET); // Pula cabeçalho
    int Contador = 0;
    // Roda enquanto Contador for menor E enquanto conseguir ler
    while (Contador < qtdSegues)
    {
        // Le o registro Atual
        RegistroSegue RegAtualS;
        leRegistroSegue(arqS, &RegAtualS);
        if (RegAtualS.removido == '1')
        {
            // Se removido, incrementa o contador (pois leu um registro) e continua
            Contador++;
            continue;
        }

        // Pega o Indice da ListaEncadeada a ser inserido na Lista de Adjacencia utilizando o mapa
        int idSeguidor = RegAtualS.idPessoaQueSegue;
        int Indice = Map[idSeguidor];

        // Declara o NoAtual e faz a procura do NomeUsuarioQueSegue que deve ser copiado (Se não encontrar pula para o proximo Registro)
        No *NoAtual = (No *)calloc(1, sizeof(No));
        long Aux = buscaBinIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueSegue);
        if (Aux == -1)
        {
            Contador++;
            continue;
        }
        long Offset = DadosIndice[Aux].byteOffset;
        fseek(arqP, Offset, SEEK_SET);
        copiaNoOffset(arqP, NoAtual->nomeUsuarioQueSegue);

        // Faz a procura do nomeUsuario pessoa que é seguida e copia
        Aux = buscaBinIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueESeguida);
        if (Aux == -1)
        {
            Contador++;
            continue;
        }
        Offset = DadosIndice[Aux].byteOffset;
        fseek(arqP, Offset, SEEK_SET);
        copiaNoOffset(arqP, NoAtual->nomeUsuarioQueESeguida);

        // Copia o restante do RegistroAtualS para o No Atual
        if (RegAtualS.dataInicioQueSegue[0] != '$')
        {
            strcpy(NoAtual->dataInicioQueSegue, RegAtualS.dataInicioQueSegue);
        }
        else
            NoAtual->dataInicioQueSegue[0] = '\0';

        if (RegAtualS.dataFimQueSegue[0] != '$')
        {
            strcpy(NoAtual->dataFimQueSegue, RegAtualS.dataFimQueSegue);
        }
        else
            NoAtual->dataFimQueSegue[0] = '\0';
        NoAtual->grauAmizade = RegAtualS.grauAmizade;

        // Se for um grafo transposto altera o Indice chave para o idPessoaQueESeguida
        if (TipoGrafo == 1)
        {
            Indice = Map[RegAtualS.idPessoaQueESeguida];
        }

        // Atualiza A Lista de Adjacencia com o NoAtual
        if (ListaDeAdjacencia[Indice].tamanho == 0)
        {
            ListaDeAdjacencia[Indice].inicio = NoAtual;
        }
        else
        {
            // Verifica se fim não é NULL por segurança
            if (ListaDeAdjacencia[Indice].fim != NULL)
                ListaDeAdjacencia[Indice].fim->prox = NoAtual;
            else
                ListaDeAdjacencia[Indice].inicio = NoAtual;
        }

        ListaDeAdjacencia[Indice].fim = NoAtual;
        // Atualiza a lista encadeada no Indice com o tamanho atual
        ListaDeAdjacencia[Indice].tamanho++;
        Contador++;
    }
}

// Retorna < 0 se d1 < d2, 0 se iguais, > 0 se d1 > d2
int compararDatas(char *d1, char *d2)
{
    // Comparar Ano
    int res = strncmp(&d1[6], &d2[6], 4);
    if (res != 0)
        return res;

    // Comparar Mês
    res = strncmp(&d1[3], &d2[3], 2);
    if (res != 0)
        return res;

    // Comparar Dia
    return strncmp(d1, d2, 2);
}

// Função que troca o Conteudo de no2 e no1
void trocaConteudo(No *no1, No *no2)
{
    // Salva o 'prox' original de cada um
    struct AuxNo *prox1 = no1->prox;
    struct AuxNo *prox2 = no2->prox;

    // Copia TUDO de no1 para uma auxiliar (copia automática dos arrays)
    No aux = *no1;

    // Troca os dados (copia automática dos arrays)
    *no1 = *no2;
    *no2 = aux;

    // Restaura os ponteiros 'prox' originais
    //(para não quebrar a corrente da lista encadeada)
    no1->prox = prox1;
    no2->prox = prox2;
}

// Função que ordena uma ListaEncadeada Interna
void ordenarListaInterna(Lista ListaAtual)
{
    // Verificação inicial básica
    if (ListaAtual.tamanho <= 1 || ListaAtual.inicio == NULL)
    {
        return;
    }

    int N = ListaAtual.tamanho;

    for (int i = 0; i < N - 1; i++)
    {
        No *at = ListaAtual.inicio;

        for (int j = 0; j < N - i - 1; j++)
        {
            // Se o contador 'tamanho' estiver errado e for maior que
            // o número real de nós evita acessar memória inválida.
            if (at == NULL || at->prox == NULL)
                break;

            int comparador = 0;

            // Se o for o TipoGrafo 0 (normal) compara por nomeUsuarioQueESeguida, se for transposto
            // compara por NomeUsuarioQueSegue
            if (TipoGrafo == 0)
                comparador = strcmp(at->nomeUsuarioQueESeguida, at->prox->nomeUsuarioQueESeguida);
            else
                comparador = strcmp(at->nomeUsuarioQueSegue, at->prox->nomeUsuarioQueSegue);

            // Comparação Secundária: Data de Início (se nomes forem iguais)
            if (comparador == 0)
            {
                comparador = compararDatas(at->dataInicioQueSegue, at->prox->dataInicioQueSegue);
            }

            // Realiza a troca se necessário
            if (comparador > 0)
            {
                trocaConteudo(at, at->prox);
            }

            // Avança o ponteiro
            at = at->prox;
        }
    }
}

// Função de comparação para o qsort
int compararListas(const void *a, const void *b)
{
    Lista *listaA = (Lista *)a;
    Lista *listaB = (Lista *)b;

    // Tratamento para listas vazias (segurança)
    if (listaA->inicio == NULL && listaB->inicio == NULL)
        return 0;
    if (listaA->inicio == NULL)
        return 1; // Lista vazia vai para o final
    if (listaB->inicio == NULL)
        return -1;

    // Compara o nome do usuário "Dono" da lista (quem segue) se TipoGrafo == 0
    // e quem e seguido se TipoGrafo == 1
    if (TipoGrafo == 0)
        return strcmp(listaA->inicio->nomeUsuarioQueSegue, listaB->inicio->nomeUsuarioQueSegue);
    return strcmp(listaA->inicio->nomeUsuarioQueESeguida, listaB->inicio->nomeUsuarioQueESeguida);
}

// Função que ordena a lista como um todo
void ordenarListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    // Ordena os indices com base no nomeUsuario
    qsort(ListaAdjacencia, qtdPessoas, sizeof(Lista), compararListas);
    for (int i = 0; i < qtdPessoas; i++)
    {
        // Ordena cada Indice com base no outro NomeUsuario
        ordenarListaInterna(ListaAdjacencia[i]);
    }
}

// Função de Inicialização da criação da Lista
Lista *criaListaAdjacencia(int Tipo, FILE *arqP, FILE *arqS, RegistroIndice *DadosIndice, int qtdPessoas, int qtdSegues)
{
    // Declara lista de Adjacencia (um vetor de ListasEncadeadas cada um com sua "KEY")
    Lista *ListaDeAdjacencia = (Lista *)calloc(qtdPessoas, sizeof(Lista));

    TipoGrafo = Tipo;
    int Map[MAXIMO];

    // Define um Mapa para cada IdPessoa (Usar para definir a "KEY" de cada ListaEncadeada)
    for (int i = 0; i < qtdPessoas; i++)
    {
        Map[DadosIndice[i].idPessoa] = i;
        ListaDeAdjacencia[i].tamanho = 0;
        ListaDeAdjacencia[i].inicio = NULL;
        ListaDeAdjacencia[i].fim = NULL;
    }
    // Copia os dados para a Lista de Adjacencia
    InsereAdjacencia(arqP, arqS, Map, ListaDeAdjacencia, qtdSegues, qtdPessoas, DadosIndice);
    return ListaDeAdjacencia;
}

void preencherNomesDono(Lista *grafo, int qtdPessoas, RegistroIndice *dadosIndice, FILE *arqPessoa) {
    int i;
    long offset;
    char nomeTemp[100];
    
    // Popula o campo nomeDono para cada vértice do grafo
    for (i = 0; i < qtdPessoas; i++) {
        offset = dadosIndice[i].byteOffset;
        fseek(arqPessoa, offset, SEEK_SET);
        copiaNoOffset(arqPessoa, nomeTemp);
        grafo[i].nomeDono = strdup(nomeTemp);
    }
}

void liberarNomesDono(Lista *grafo, int qtdPessoas) {
    int i;
    
    // Libera a memória alocada para os nomes dos donos
    for (i = 0; i < qtdPessoas; i++) {
        if (grafo[i].nomeDono != NULL) {
            free(grafo[i].nomeDono);
            grafo[i].nomeDono = NULL;
        }
    }
}
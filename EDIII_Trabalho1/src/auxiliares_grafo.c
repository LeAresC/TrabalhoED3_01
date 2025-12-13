#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#include "io_cabecalho.h"
#include "io_registro.h"
#define MAXIMO 5000

int TipoGrafo = 0;

void imprimeListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    for (int i = 0; i < qtdPessoas; i++)
    {
        int qtdSegue = ListaAdjacencia[i].tamanho;
        No *at = ListaAdjacencia[i].inicio;
        for (int j = 0; j < qtdSegue; j++)
        {
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
            if (strcmp(at->dataInicioQueSegue, "-") != 0)
                printf("%s, ", at->dataInicioQueSegue);
            else
                printf("NULO, ");
            if (strcmp(at->dataFimQueSegue, "-") != 0)
                printf("%s, ", at->dataFimQueSegue);
            else
                printf("NULO, ");
            if (at->grauAmizade != '-')
                printf("%c", at->grauAmizade);
            else
                printf("NULO");
            printf("\n");
            at = at->prox;
        }
        printf("\n");
    }
}

void copiaNoOffset(FILE *arqP, char *NomeUsuario)
{
    RegistroPessoa registroAtual;

    // 2. Verifica se a leitura do cabeçalho funcionou
    if (fread(&registroAtual.removido, sizeof(char), 1, arqP) != 1)
    {
        registroAtual.removido = 'X'; // Marca erro
    }

    fread(&registroAtual.tamanhoRegistro, sizeof(int), 1, arqP);

    fread(&registroAtual.idPessoa, sizeof(int), 1, arqP);
    fread(&registroAtual.idadePessoa, sizeof(int), 1, arqP);

    // 3. Leitura Crítica: Tamanho do Nome
    fread(&registroAtual.tamanhoNomePessoa, sizeof(int), 1, arqP);
    char temp[100];
    if (registroAtual.tamanhoNomePessoa > 0)
    {
        fread(temp, sizeof(char), registroAtual.tamanhoNomePessoa, arqP);
    }

    // 5. Repete para NomeUsuario
    fread(&registroAtual.tamanhoNomeUsuario, sizeof(int), 1, arqP);

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
    // Loop de segurança: roda enquanto Contador for menor E enquanto conseguir ler
    while (Contador < qtdSegues)
    {
        RegistroSegue RegAtualS;
        leRegistroSegue(arqS, &RegAtualS);
        // 2. CORREÇÃO DO LOOP INFINITO
        if (RegAtualS.removido == '1')
        {
            // Se removido, incrementa o contador (pois leu um registro) e continua
            Contador++;
            continue;
        }

        int idSeguidor = RegAtualS.idPessoaQueSegue;
        int Indice = Map[idSeguidor];

        No *NoAtual = (No *)malloc(MAXIMO);
        long Aux = buscaBinIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueSegue);
        if (Aux == -1)
            continue;
        long Offset = DadosIndice[Aux].byteOffset;
        fseek(arqP, Offset, SEEK_SET);
        copiaNoOffset(arqP, NoAtual->nomeUsuarioQueSegue);

        Aux = buscaBinIndice(DadosIndice, qtdPessoas, RegAtualS.idPessoaQueESeguida);
        if (Aux == -1)
            continue;
        Offset = DadosIndice[Aux].byteOffset;
        fseek(arqP, Offset, SEEK_SET);
        copiaNoOffset(arqP, NoAtual->nomeUsuarioQueESeguida);

        strncpy(NoAtual->dataInicioQueSegue, RegAtualS.dataInicioQueSegue, 10);
        NoAtual->dataInicioQueSegue[10] = '\0';

        strncpy(NoAtual->dataFimQueSegue, RegAtualS.dataFimQueSegue, 10);
        NoAtual->dataFimQueSegue[10] = '\0';

        NoAtual->grauAmizade = RegAtualS.grauAmizade;

        if (TipoGrafo == 1)
        {
            Indice = Map[RegAtualS.idPessoaQueESeguida];
        }
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
                ListaDeAdjacencia[Indice].inicio = NoAtual; // Fallback caso inconsistente
        }

        ListaDeAdjacencia[Indice].fim = NoAtual;
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

void trocaConteudo(No *no1, No *no2)
{
    // CUIDADO: Precisamos preservar o ponteiro 'prox', 
    // pois queremos trocar apenas os DADOS, não a ligação da lista.
    
    // 1. Salva o 'prox' original de cada um
    struct AuxNo *prox1 = no1->prox;
    struct AuxNo *prox2 = no2->prox;

    // 2. Copia TUDO de no1 para uma auxiliar (copia automática dos arrays)
    No aux = *no1;

    // 3. Troca os dados (copia automática dos arrays)
    *no1 = *no2;
    *no2 = aux;

    // 4. Restaura os ponteiros 'prox' originais 
    // (para não quebrar a corrente da lista encadeada)
    no1->prox = prox1;
    no2->prox = prox2;
}
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
            // --- PROTEÇÃO CONTRA SEGMENTATION FAULT ---
            // Se o contador 'tamanho' estiver errado e for maior que 
            // o número real de nós, isso evita acessar memória inválida.
            if (at == NULL || at->prox == NULL) break; 

            int comparador = 0;

            // Comparação Primária: Nomes
            // Tipo 0 (Func 11): Ordena por quem É SEGUIDO [cite: 483]
            // Tipo 1 (Func 12): Ordena por quem SEGUE [cite: 536]
            if (TipoGrafo == 0)
                comparador = strcmp(at->nomeUsuarioQueESeguida, at->prox->nomeUsuarioQueESeguida);
            else
                comparador = strcmp(at->nomeUsuarioQueSegue, at->prox->nomeUsuarioQueSegue);

            // Comparação Secundária: Data de Início (se nomes forem iguais)
            if (comparador == 0)
            {
                comparador = compararDatas(at->dataInicioQueSegue, at->prox->dataInicioQueSegue);
                
                // Comparação Terciária: Data de Fim (se datas de início forem iguais)
                // Recomendado pelas regras gerais de ordenação 
                if (comparador == 0) {
                     comparador = compararDatas(at->dataFimQueSegue, at->prox->dataFimQueSegue);
                }
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

void ordenarListaAdjacencia(Lista *ListaAdjacencia, int qtdPessoas)
{
    qsort(ListaAdjacencia, qtdPessoas, sizeof(Lista), compararListas);
    for (int i = 0; i < qtdPessoas; i++)
    {
        ordenarListaInterna(ListaAdjacencia[i]);
    }
}

Lista *criaListaAdjacencia(int Tipo, FILE *arqP, FILE *arqS, RegistroIndice *DadosIndice, int qtdPessoas, int qtdSegues)
{
    Lista *ListaDeAdjacencia = (Lista *)malloc(sizeof(Lista) * qtdPessoas);

    TipoGrafo = Tipo;
    int Map[MAXIMO];

    for (int i = 0; i < qtdPessoas; i++)
    {
        Map[DadosIndice[i].idPessoa] = i;
        ListaDeAdjacencia[i].tamanho = 0;
        ListaDeAdjacencia[i].inicio = NULL;
        ListaDeAdjacencia[i].fim = NULL;
    }
    InsereAdjacencia(arqP, arqS, Map, ListaDeAdjacencia, qtdSegues, qtdPessoas, DadosIndice);
    return ListaDeAdjacencia;
}
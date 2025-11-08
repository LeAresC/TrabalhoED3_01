#include "utils.h"
#include <stdlib.h>
#include <string.h>

char* obterProximoCampo(char **linha) {
    //Declaração de variáveis
    char *inicio, *p, *campo;
    int tamanho;

    // Verifica se a linha está vazia
    if (**linha == '\0') {
        return NULL;
    }

    // Ponteiros para o início do campo e para percorrer a linha
    inicio = *linha;
    p = inicio;

    // Avançar até encontrar a vírgula ou o final da string
    while (*p != ',' && *p != '\0') {
        p++;
    }

    // Calcular o tamanho do campo e alocar memória para ele
    tamanho = p - inicio;
    campo = (char*) malloc(tamanho + 1);
    strncpy(campo, inicio, tamanho);
    campo[tamanho] = '\0';

    if (*p == ',') {
        *linha = p + 1; // Mais campos na linha
    } else {
        *linha = p; // Acabou a linha
    }

    return campo;
}

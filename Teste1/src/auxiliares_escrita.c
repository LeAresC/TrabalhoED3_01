#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_structs.h"
#define INF 1e9

// Função de comparação para um array de PONTEIROS (RegistroIndice **)
int compararIndicePorID(const void *a, const void *b) {
    
    // 1. Converte 'a' e 'b' de volta para seus tipos reais:
    //    ponteiros para ponteiros (RegistroIndice **)
    const RegistroIndice **ptrA = (const RegistroIndice **)a;
    const RegistroIndice **ptrB = (const RegistroIndice **)b;

    // 2. Obtém os ponteiros para as structs reais
    //    (dereferenciando uma vez)
    const RegistroIndice *regA = *ptrA;
    const RegistroIndice *regB = *ptrB;

    // 3. Agora sim, compara os campos das structs
    if (regA->idPessoa < regB->idPessoa) {
        return -1;
    } else if (regA->idPessoa > regB->idPessoa) {
        return 1;
    } else {
        return 0;
    }
}
int escreveIndice(char *arquivoIndice, RegistroIndice **DadosIndice, int tamanhoArquivoIndice)
{
    FILE *arqI = fopen(arquivoIndice, "wb");
    if (arqI == NULL)
        return 0;
    CabecalhoIndice Cabecalho;
    Cabecalho.status = '1';
    memset(Cabecalho.lixo, '$', sizeof(Cabecalho.lixo));
    fwrite(&Cabecalho.status, sizeof(Cabecalho.status), 1, arqI);
    fwrite(Cabecalho.lixo, sizeof(Cabecalho.lixo), 1, arqI);

    for (int i = 0; i < tamanhoArquivoIndice; i++)
    {
            fwrite(&DadosIndice[i]->idPessoa, sizeof(int), 1, arqI);
            fwrite(&DadosIndice[i]->byteOffset, sizeof(long long int), 1, arqI);
    }
    fclose(arqI);
    return 1;
}
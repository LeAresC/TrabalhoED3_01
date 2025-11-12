#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "io_cabecalho.h"
#include "io_registro.h"
#include "utils_parse.h"
#include "utils_print.h"
#include "utils_busca.h"
#define MAXIMO 500

int buscaEmArquivo(char *nomeArquivoDados, char *nomeArquivoIndice, int quantidade) {
    // Declaração de variáveis
    FILE *arquivoDados, *arquivoIndice;
    CabecalhoPessoa cabecalho;
    RegistroIndice *registroIndice;
    int quantidadeIndices, i, j, matchCount;
    char nomeCampo[MAXIMO], valorCampo[MAXIMO];
    RegistroPessoa **matches;

    // Abrir os arquivos e verificar erros
    arquivoDados = fopen(nomeArquivoDados, "rb");
    arquivoIndice = fopen(nomeArquivoIndice, "rb");
    if (arquivoDados == NULL || arquivoIndice == NULL) {
        if (arquivoDados) fclose(arquivoDados);
        if (arquivoIndice) fclose(arquivoIndice);
        return 0;
    }

    // Lê o registro de cabeçalho
    cabecalho = leCabecalhoPessoa(arquivoDados);

    // Verifica o status do arquivo
    if (cabecalho.status == '0') {
        fclose(arquivoDados);
        fclose(arquivoIndice);
        return 0;
    }

    // Carrega o índice para a RAM (correção crítica de performance)
    fseek(arquivoIndice, 12, SEEK_SET);
    quantidadeIndices = cabecalho.quantidadePessoas + cabecalho.quantidadeRemovidos;
    registroIndice = malloc(quantidadeIndices * sizeof(RegistroIndice));
    
    // Loop para carregar todos os registros de índice
    for (i = 0; i < quantidadeIndices; i++) {
        leRegistroIndice(arquivoIndice, &registroIndice[i]);
    }
    
    // Fecha o arquivo de índice pois não é mais necessário
    fclose(arquivoIndice);

    // Loop principal de consultas
    for (i = 0; i < quantidade; i++) {
        // Le e separa o nome campo do valor campo do input
        leCriterioBusca(nomeCampo, valorCampo);

        // Chama a função central buscaPessoas
        matchCount = 0;
        matches = buscaPessoas(arquivoDados, registroIndice, quantidadeIndices, nomeCampo, valorCampo, &matchCount);

        // Processa os resultados
        if (matchCount == 0) {
            erroRegistro();
        } else {
            // Loop para imprimir todos os registros encontrados
            for (j = 0; j < matchCount; j++) {
                imprimirSaida(matches[j]);
                // Libera a memória alocada para o registro lido
                liberaRegistroPessoa(matches[j]);
            }
            // Libera a lista de matches
            free(matches);
        }
    }

    // Libera memória e fecha os arquivos
    free(registroIndice);
    fclose(arquivoDados);
    // Retorna sucesso
    return 1;
}

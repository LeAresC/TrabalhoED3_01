#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "io_registro.h"
#include "io_cabecalho.h"
#include "utils_print.h"

#define MAXIMO 500


int lerArquivoBinario(char *nomeDoArquivo) {
    // Declaração de variáveis
    FILE *arquivoDados;
    CabecalhoPessoa cabecalho;
    RegistroPessoa *registroAtual;

    // Abre o arquivo e verifica se conseguiu abrir corretamente
    arquivoDados = fopen(nomeDoArquivo, "rb");
    if (arquivoDados == NULL) {
        return 0;
    }

    // Lê o registro de cabeçalho
    cabecalho = leCabecalhoPessoa(arquivoDados);

    // Verifica o status do arquivo
    if (cabecalho.status == '0') {
        fclose(arquivoDados);
        return 0;
    }

    // Se não houver registros retorna 1
    if (cabecalho.quantidadePessoas == 0) {
        fclose(arquivoDados);
        return 1;
    }

    // Lê todos os registros e imprime na saida
    while ((registroAtual = leRegistroPessoa(arquivoDados)) != NULL) {
        // Se estiver removido pula para o proximo
        if (registroAtual->removido == '1') {
            fseek(arquivoDados, registroAtual->tamanhoRegistro, SEEK_CUR);
        }
        // Imprime o registro completo
        else {
            imprimirSaida(registroAtual);
        }
        // Libera a memória alocada para o registro lido
        liberaRegistroPessoa(registroAtual);
    }

    // Fecha o arquivo
    fclose(arquivoDados);
    // Retorna sucesso
    return 2;
}

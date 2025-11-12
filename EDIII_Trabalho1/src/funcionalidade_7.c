#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliares_busca.h"
#include "auxiliares_escrita.h"
#include "utilidades.h"
#define MAXIMO 2000
#define INF 1e9

int atualizaArquivos(char *arquivoDados, char *arquivoIndice, int N)
{
    // Abre os arquivos e verifica se conseguiu abrir
    FILE *arqD = fopen(arquivoDados, "r+b");
    FILE *arqI = fopen(arquivoIndice, "r+b");
    if (arqD == NULL || arqI == NULL)
    {
        return 0;
    }
    // Le o cabecalho do arquivo pessoa
    CabecalhoPessoa *CabecalhoP = leCabecalhoPessoa(arqD);

    // Passa todos os dados do arquivo de Indice para a RAM
    RegistroIndice **DadosIndice = leArquivoIndice(arqI, CabecalhoP->quantidadePessoas);

    // Atualiza a consistência do arquivo ('0' inconsistente)
    atualizaConsistencia(arqD, arqI);

    // Declara uma variavel que ira armazenar o tamanho do indice em RAM
    int tamanhoIndice = CabecalhoP->quantidadePessoas;
    for (int i = 0; i < N; i++)
    {

        // Declara variáveis e le o Input
        int cnt; scanf("%d", &cnt);
        char nomeCampo1[MAXIMO];
        char valorCampo1[MAXIMO];
        char nomeCampo2[MAXIMO];
        char valorCampo2[MAXIMO];

        leInput(nomeCampo1, valorCampo1);
        leInput(nomeCampo2, valorCampo2);

        // Declara e le os offsets que satisfazem o input
        long *Offsets = buscaDados(arqD, DadosIndice, nomeCampo1, valorCampo1);
        // (0 == idPessoa, 1 = idadePessoa, 2 = nomePessoa, 3 = nomeUsuario)
        int campoBusca = determinarCampoBusca(nomeCampo2);
        for (int j = 0; j < MAXIMO; j++)
        {
            if (Offsets[j] == -1)
                break;

            // Le o registro do offset atual, ele será atualizado e reinserido
            fseek(arqD, Offsets[j], SEEK_SET);
            RegistroPessoa *RegistroAtual = leRegistroPessoa(arqD);

            // Variáveis auxiliares
            int idAnterior = RegistroAtual->idPessoa;
            int OffsetdeAtualização = Offsets[j];
            int tamanhoAnterior = RegistroAtual->tamanhoRegistro;
            int tamanhoNec = RegistroAtual->tamanhoRegistro;

            // Atualiza o registroAtual conforme o filtro para ser reinserido
            if (campoBusca == 0)
            {
                RegistroAtual->idPessoa = atoi(valorCampo2);
            }
            else if (campoBusca == 1)
            {
                // Verifica se o campo é nulo para atualizar a idade 
                if (strlen(valorCampo2) != 0)
                    RegistroAtual->idadePessoa = atoi(valorCampo2);
                else
                    RegistroAtual->idadePessoa = -1;
            }
            else if (campoBusca == 2)
            {
                // Copia o nomePessoa do campo para o registro
                strcpy(RegistroAtual->nomePessoa, valorCampo2);
                RegistroAtual->tamanhoNomePessoa = strlen(valorCampo2);
                // Tamanho Necessário para o novo registro
                tamanhoNec = 16 + RegistroAtual->tamanhoNomePessoa + RegistroAtual->tamanhoNomeUsuario;
            }
            else if (campoBusca == 3)
            {
                // Copia o nomeUsuario do campo para o registro
                strcpy(RegistroAtual->nomeUsuario, valorCampo2);
                RegistroAtual->tamanhoNomeUsuario = strlen(valorCampo2);
                // Tamanho Necessário para o novo registro
                tamanhoNec = 16 + RegistroAtual->tamanhoNomePessoa + RegistroAtual->tamanhoNomeUsuario;
            }
            // Verifica se o novo registro cabe no lugar do registro anterior, se não, o remove e faz atualizações
            if (tamanhoNec > tamanhoAnterior)
            {
                //Remove o registro no offset
                removeRegistroOffsetPessoa(arqD, Offsets[j]);
            
                // Atualiza o tamanho 
                RegistroAtual->tamanhoRegistro = tamanhoNec;

                // Atualiza o offset de inserção 
                OffsetdeAtualização = CabecalhoP->proxByteOffset;

                // Atualiza o cabecalho em RAM
                CabecalhoP->quantidadeRemovidos++;
                CabecalhoP->proxByteOffset += 5 + RegistroAtual->tamanhoRegistro;
            }
            // Atualiza o arquivo pessoa
            inserePessoa(arqD, OffsetdeAtualização, RegistroAtual);
            // Atualiza o indice em RAM mantendo a ordenação
            atualizaNoIndice(DadosIndice, tamanhoIndice, RegistroAtual->idPessoa, OffsetdeAtualização, idAnterior);

            // Libera a memória
            free(RegistroAtual->nomePessoa);
            free(RegistroAtual->nomeUsuario);
            free(RegistroAtual);
        }

        // Atualiza o cabecalho em disco
        atualizaCabecalhoPessoa(arqD, CabecalhoP);
    }
    // Fecha os arquivos, libera a memória e escreve o arquivo de Indice em RAM no Disco
    fclose(arqI);
    escreveIndice(arquivoIndice, DadosIndice, CabecalhoP->quantidadePessoas);
    for (int i = 0; i < tamanhoIndice; i++)
    {
        free(DadosIndice[i]);
    }
    free(DadosIndice);
    free(CabecalhoP);
    fclose(arqD);
    return 1;
}
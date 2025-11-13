#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

typedef struct {
    char status; // 1 byte: '0' inconsistente, '1' consistente
    int quantidadePessoas; // 4 bytes
    int quantidadeRemovidos; // 4 bytes
    long proxByteOffset; // 8 bytes
} CabecalhoPessoa;

typedef struct {
    char removido; // 1 byte: '0' ativo, '1' removido
    int tamanhoRegistro; // 4 bytes
    int idPessoa; // 4 bytes
    int idadePessoa; // 4 bytes
    int tamanhoNomePessoa; // 4 bytes
    int tamanhoNomeUsuario; // 4 bytes
    char *nomePessoa; // variável
    char *nomeUsuario; // variável
} RegistroPessoa;

typedef struct {
    char status; // 1 byte: '0' inconsistente, '1' consistente
    char lixo[11]; // 11 bytes de lixo preenchidos com '$'
} CabecalhoIndice;

typedef struct {
    int idPessoa; // 4 bytes
    long byteOffset; // 8 bytes
} RegistroIndice;

typedef struct {
    char status; // 1 byte: '0' inconsistente, '1' consistente
    int quantidadePessoas; // 4 bytes
    int proxRRN; // 4 bytes
} CabecalhoSegue;

typedef struct {
    char removido; // 1 byte: '0' ativo, '1' removido
    int idPessoaQueSegue; // 4 bytes
    int idPessoaQueESeguida; // 4 bytes
    char dataInicioQueSegue[11]; // 11 bytes (10 para "DD/MM/AAAA")
    char dataFimQueSegue[11]; // 11 bytes (10 para "DD/MM/AAAA")
    char grauAmizade; // 1 byte ('0' a '2' ou '$' para nulo)
} RegistroSegue;

#endif // DATA_STRUCTS_H

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

#endif // DATA_STRUCTS_H




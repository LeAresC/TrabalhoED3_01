#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

typedef struct {
    char status;
    int quantidadePessoas;
    int quantidadeRemovidos;
    long proxByteOffset;
} PersonDataHeader;

typedef struct {
    char removido;
    int tamanhoRegistro;
    int idPessoa;
    int idadePessoa;
    int tamanhoNomePessoa;
    int tamanhoNomeUsuario;
    char *nomePessoa;
    char *nomeUsuario;
} PersonDataRecord;

typedef struct {
    char status;
    char lixo[11];
} IndexHeader;

typedef struct {
    int idPessoa;
    long byteOffset;
} IndexRecord;


#endif // DATA_STRUCTS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "create_table.h"

static int compareIndexRecords(const void *a, const void *b) {
    const IndexRecord *recA = (const IndexRecord *)a;
    const IndexRecord *recB = (const IndexRecord *)b;
    
    if (recA->idPessoa < recB->idPessoa) return -1;
    if (recA->idPessoa > recB->idPessoa) return 1;
    return 0;
}

static char* getNextField(char **line) {
    if (**line == '\0') {
        return NULL;
    }

    char *start = *line;
    char *p = start;
    
    while (*p != ',' && *p != '\0') {
        p++;
    }

    int len = p - start;
    char *field = (char*) malloc(len + 1);
    strncpy(field, start, len);
    field[len] = '\0';

    if (*p == ',') {
        *line = p + 1;
    } else {
        *line = p;
    }

    return field;
}

PersonDataRecord* readCsvRecord(FILE *csvFile) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), csvFile) == NULL) {
        return NULL;
    }

    buffer[strcspn(buffer, "\r\n")] = 0;
    
    char *line = buffer;

    PersonDataRecord *record = (PersonDataRecord*) malloc(sizeof(PersonDataRecord));
    if (record == NULL) return NULL;

    char *field;

    field = getNextField(&line);
    record->idPessoa = atoi(field);
    free(field);

    field = getNextField(&line);
    if (field == NULL || strlen(field) == 0) {
        record->nomePessoa = malloc(1);
        record->nomePessoa[0] = '\0';
        record->tamanhoNomePessoa = 0;
    } else {
        record->nomePessoa = malloc(strlen(field) + 1);
        strcpy(record->nomePessoa, field);
        record->tamanhoNomePessoa = strlen(record->nomePessoa);
    }
    free(field);
    
    field = getNextField(&line);
    if (field == NULL || strlen(field) == 0) {
        record->idadePessoa = -1;
    } else {
        record->idadePessoa = atoi(field);
    }
    free(field);

    field = getNextField(&line);
    record->nomeUsuario = malloc(strlen(field) + 1);
    strcpy(record->nomeUsuario, field);
    record->tamanhoNomeUsuario = strlen(record->nomeUsuario);
    free(field);

    
    record->removido = '0';
    record->tamanhoRegistro = 0;

    return record;
}


void createTable(char *csvFilename, char *dataFilename, char *indexFilename) {
    FILE *csvFile = fopen(csvFilename, "r");
    FILE *dataFile = fopen(dataFilename, "wb");
    FILE *indexFile = fopen(indexFilename, "wb");

    if (csvFile == NULL || dataFile == NULL || indexFile == NULL) {
        printf("Falha no processamento do arquivo.\n");
        if (csvFile) fclose(csvFile);
        if (dataFile) fclose(dataFile);
        if (indexFile) fclose(indexFile);
        return;
    }

    PersonDataHeader dataHeader;
    memset(&dataHeader, 0, sizeof(PersonDataHeader));
    dataHeader.status = '0';
    dataHeader.proxByteOffset = 17;
    
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, dataFile);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, dataFile);
    fwrite(&dataHeader.quantidadeRemovidos, sizeof(dataHeader.quantidadeRemovidos), 1, dataFile);
    fwrite(&dataHeader.proxByteOffset, sizeof(dataHeader.proxByteOffset), 1, dataFile);

    IndexHeader indexHeader;
    memset(&indexHeader, 0, sizeof(IndexHeader));
    indexHeader.status = '0';
    memset(indexHeader.lixo, '$', 11);
    fwrite(&indexHeader, sizeof(IndexHeader), 1, indexFile);

    char csvHeader[256];
    fgets(csvHeader, sizeof(csvHeader), csvFile);

    IndexRecord *indexArray = NULL;
    int indexCount = 0;
    int indexCapacity = 0;

    PersonDataRecord *record;
    while ((record = readCsvRecord(csvFile)) != NULL) {
        long currentByteOffset = dataHeader.proxByteOffset;

        if (indexCount == indexCapacity) {
            indexCapacity = (indexCapacity == 0) ? 8 : indexCapacity * 2;
            indexArray = realloc(indexArray, indexCapacity * sizeof(IndexRecord));
        }
        indexArray[indexCount].idPessoa = record->idPessoa;
        indexArray[indexCount].byteOffset = currentByteOffset;
        indexCount++;

        record->tamanhoRegistro = sizeof(record->idPessoa) +
                                  sizeof(record->idadePessoa) +
                                  sizeof(record->tamanhoNomePessoa) + record->tamanhoNomePessoa +
                                  sizeof(record->tamanhoNomeUsuario) + record->tamanhoNomeUsuario;

        long totalRecordSize = sizeof(record->removido) + sizeof(record->tamanhoRegistro) + record->tamanhoRegistro;

        fseek(dataFile, currentByteOffset, SEEK_SET);

        fwrite(&record->removido, sizeof(record->removido), 1, dataFile);
        fwrite(&record->tamanhoRegistro, sizeof(record->tamanhoRegistro), 1, dataFile);
        fwrite(&record->idPessoa, sizeof(record->idPessoa), 1, dataFile);
        fwrite(&record->idadePessoa, sizeof(record->idadePessoa), 1, dataFile);
        fwrite(&record->tamanhoNomePessoa, sizeof(record->tamanhoNomePessoa), 1, dataFile);

        if (record->tamanhoNomePessoa > 0) {
            fwrite(record->nomePessoa, record->tamanhoNomePessoa, 1, dataFile);
        }
        fwrite(&record->tamanhoNomeUsuario, sizeof(record->tamanhoNomeUsuario), 1, dataFile);
        fwrite(record->nomeUsuario, record->tamanhoNomeUsuario, 1, dataFile);

        dataHeader.quantidadePessoas++;
        dataHeader.proxByteOffset += totalRecordSize;

        free(record->nomePessoa);
        free(record->nomeUsuario);
        free(record);
    }

    qsort(indexArray, indexCount, sizeof(IndexRecord), compareIndexRecords);

    for (int i = 0; i < indexCount; i++) {
        fwrite(&indexArray[i], sizeof(IndexRecord), 1, indexFile);
    }

    dataHeader.status = '1';
    fseek(dataFile, 0, SEEK_SET);
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, dataFile);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, dataFile);
    fwrite(&dataHeader.quantidadeRemovidos, sizeof(dataHeader.quantidadeRemovidos), 1, dataFile);
    fwrite(&dataHeader.proxByteOffset, sizeof(dataHeader.proxByteOffset), 1, dataFile);

    indexHeader.status = '1';
    fseek(indexFile, 0, SEEK_SET);
    fwrite(&indexHeader, sizeof(IndexHeader), 1, indexFile);

    free(indexArray);
    fclose(csvFile);
    fclose(dataFile);
    fclose(indexFile);
}

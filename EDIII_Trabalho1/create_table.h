#ifndef CREATE_TABLE_H
#define CREATE_TABLE_H


#include "data_structs.h"
#include <stdio.h>

void createTable(char *csvFilename, char *dataFilename, char *indexFilename);

PersonDataRecord* readCsvRecord(FILE *csvFile);

#endif // CREATE_TABLE_H

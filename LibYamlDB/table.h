#ifndef LIBYAMLDB_TABLE
#define LIBYAMLDB_TABLE

#include <stdio.h>

#include "attribut.h"

typedef struct Table Table;

#include "occurence.h"

struct Table {
    FILE* file;
    char* name;
    int lengthAttributes;
    Attribut** attributes;
    int lengthOccurences;
    int capacityOccurences;
    Occurence** occurences;
};

#include "database.h"
#include "occurence.h"

Table* tableNew(char* databaseName, char* name, int lengthAttributes, Attribut** attributes);
Table* tableOpen(char* databaseName, char* tableName);

void tableRetrieveAttributes(Table* table);
void tableRetrieveOccurences(Table* table);

int tableIs(char* fileName);

void tableWriteAttributes(Table* table);
void tableWriteOccurence(Table* table, Occurence* occurence);
void tableInsertOccurence(Table* table, Occurence* occurence);

void tableRemoveOccurence(Table* table, Occurence* occurence);
void tableWriteModifications(Database* database, Table* table);

void tableFree(Table* table);
void tableDeleteFile(Database* database, Table* table);

#endif
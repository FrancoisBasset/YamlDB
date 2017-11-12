#ifndef LIBYAMLDB_TABLE
#define LIBYAMLDB_TABLE

#include <stdio.h>

#include "attribut.h"

typedef struct Table {
    FILE* file;
    char* name;
    int lengthAttributes;
    Attribut** attributes;
} Table;

#include "database.h"

Table* tableNew(char* databaseName, char* name, int lengthAttributes, Attribut** attributes);
void tableWriteAttributes(Table* table);

void tableFree(Table* table);
void tableDeleteFile(Database* database, Table* table);
Table* tableOpen(char* databaseName, char* tableName);

#endif
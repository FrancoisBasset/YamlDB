#ifndef LIBYAMLDB_TABLE
#define LIBYAMLDB_TABLE

#include <stdio.h>


#include "attribut.h"

typedef struct Table {
    FILE* file;
    char* name;
    Attribut* attributes;
} Table;

#include "database.h"

Table* newTable(char* name, Attribut* attributes);

//void freeTable(Table* table);
void destroyTable(Database* database, Table* table);

#endif
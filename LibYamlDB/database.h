#ifndef LIBYAMLDB_DATABASE
#define LIBYAMLDB_DATABASE

#include <stdio.h>

typedef struct Database Database;

#include "table.h"

struct Database {
    FILE* file;
    Table** tables;
    int lengthTables;
};


Database* newDatabase(char* name);
int writeNewDatabase(Database* database);

Database* openDatabase(char* name);
char** getTablesNames(Database* database);

void addNewTable(Database* database, Table* table);

int freeDatabase(Database* database);
void destroyDatabase(Database* database);

#endif
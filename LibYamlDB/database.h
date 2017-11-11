#ifndef LIBYAMLDB_DATABASE
#define LIBYAMLDB_DATABASE

#include <stdio.h>

typedef struct Database Database;



#include "table.h"

struct Database {
    char* name;
    FILE* file;
    Table** tables;
    int lengthTables;
    int capacityTables;
};

Database* databaseNew(char* name);
int databaseWriteNew(Database* database, char* fileName);

Database* databaseOpen(char* name);
int databaseRetrieveTables(Database* database);

int tableExists(Database* database, char* tableName);
void databaseAddNewTable(Database* database, Table* table);
void databaseDeleteTable(Database* database, char* tableName);

int databaseFree(Database* database);
int databaseDelete(Database* database);

#endif
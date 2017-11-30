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
Database* databaseOpen(char* name);

int databaseIs(char* fileName);
int tableExists(Database* database, char* tableName);

int databaseWriteNew(Database* database, char* fileName);

int databaseRetrieveTables(Database* database);
Table* databaseGetTable(Database* database, char* tableName);

void databaseAddNewTable(Database* database, Table* table);
void databaseDeleteTable(Database* database, char* tableName);

int databaseFree(Database* database);
int databaseDelete(Database* database);

void databaseWriteModifications(Database* database);

char** databaseShowDatabases(int* databaseLength);

#endif
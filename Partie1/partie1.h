#ifndef PARTIE1
#define PARTIE1

#include "../LibYamlDB/database.h"
#include "../LibYamlDB/attribut.h"

void launch();
void menu(char* databaseName, int baseOpened);
Database* mainNewDatabase(int* baseOpened);
Database* mainOpenDatabase(int* baseOpened);
void mainDeleteDatabase(Database* database, int* baseOpened);
void mainDatabaseAddNewTable(Database* database);
void show(Database* db);
void mainDatabaseDeleteTable(Database* database);

#endif

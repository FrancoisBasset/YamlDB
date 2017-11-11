#ifndef PARTIE1_H
#define PARTIE1_H

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
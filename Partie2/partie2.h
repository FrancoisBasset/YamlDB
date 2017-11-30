#ifndef PARTIE2_PARTIE2
#define PARTIE2_PARTIE2

#include "../LibYamlDB/database.h"
#include "../LibYamlDB/commandType.h"
#include "../LibYamlDB/table.h"
#include "../LibYamlDB/attribut.h"
#include "../LibYamlDB/yamlUtil.h"
#include "../LibYamlDB/occurenceValue.h"
#include "../LibYamlDB/occurence.h"

void launch();

Database* mainUseDatabase(Database* database, char* command);

void mainShowDatabases();
void mainShowTables(Database* database);
void mainDescribeTable(Database* database, char* command);
void mainInsertIntoTables(Database* database, char* command);

Table* mainGetInsertTable(Database* database, char* command);
Occurence* mainGetOccurenceInsertValues(char* command);

#endif
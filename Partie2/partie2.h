#ifndef PARTIE2_PARTIE2
#define PARTIE2_PARTIE2

#include "../LibYamlDB/database.h"
#include "../LibYamlDB/commandType.h"
#include "../LibYamlDB/table.h"
#include "../LibYamlDB/attribut.h"
#include "../LibYamlDB/yamlUtil.h"
#include "../LibYamlDB/occurenceValue.h"
#include "../LibYamlDB/occurence.h"
#include "../LibYamlDB/condition.h"
#include "../LibYamlDB/affectation.h"

void launch();

Database* mainUseDatabase(Database* database, char* command);

void mainShowDatabases();
void mainShowTables(Database* database);
void mainDescribeTable(Database* database, char* command);

void mainInsertIntoTables(Database* database, char* command);
Table* mainGetInsertTable(Database* database, char* command);
Occurence* mainGetOccurenceInsertValues(char* command);

void mainDeleteFromTable(Database* database, char* command);
Table* mainGetDeleteTable(Database* database, char* command);
Occurence** mainGetOccurencesFromJointures(Table* table, char* command, int* nbOccurencesRes);

void mainUpdateTable(Database* database, char* command);
Table* mainGetTableFromCommand(Database* database, char* command, CommandType commandType);

void mainSelectFrom(Database* database, char* command);
char** mainGetSelectAttributes(Table* table, char* command, int* nbSelectors);

#endif
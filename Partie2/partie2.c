#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partie2.h"
#include "../LibYamlDB/condition.h"

void launch() {
    printf("Hello, exit to quit\n");

    Database* database = NULL;
    CommandType commandType;

    char *command = malloc(sizeof(char) * 500);
    strcpy(command, "");

    while (strcmp(command, "exit") != 0) {
        printf(">>> ");

        strcpy(command, "\n");
        scanf("%[^\n]s", command);
        trim(command);
        commandType = getCommandType(command);

        switch (commandType) {
            case CreateDatabase:
                printf("CreateDatabase");
                break;
            case CreateTable:
                printf("CreateTable");
                break;
            case DropDatabase:
                printf("DropDatabase");
                break;
            case DropTable:
                printf("DropTable");
                break;
            case Use:
                database = mainUseDatabase(database, command);
                break;
            case Select:
                printf("Select\n");
                break;
            case Insert:
                mainInsertIntoTables(database, command);
                break;
            case Update:
                printf("Update\n");
                break;
            case Delete:
                mainDeleteFromTable(database, command);
                break;
            case ShowDatabases:
                mainShowDatabases();
                break;
            case ShowTables:
                mainShowTables(database);
                break;
            case Describe:
                mainDescribeTable(database, command);
                break;
            case Help:
                printf("Help\n");
                break;
            case Exit:
                printf("Bye\n");
                break;
            case Unknown:
                printf("?\n");
                break;
            case Null:
                break;
        }

        int c;
        while((c=getchar()) != EOF && c != '\n');
    }

    free(command);
    databaseFree(database);
}

void mainShowDatabases() {
    int dbLength = 0;
    char** dbNames = databaseShowDatabases(&dbLength);

    int i;

    for (i = 0; i < dbLength; i++) {
        printf("%s\n", dbNames[i]);
    }

    for (i = 0; i < dbLength; i++) {
        free(dbNames[i]);
    }
    free(dbNames);
}

void mainShowTables(Database* database) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    for (int i = 0; i < database->lengthTables; i++) {
        printf("%s\n", database->tables[i]->name);
    }
}

/*Database* mainCreateDatabase(char* command) {

}*/

Database* mainUseDatabase(Database* database, char* command) {
    char* tmp = strtok(command, " ");

    for (int i = 1; i < 2; i++) {
        tmp = strtok(NULL, " ");
    }

    char* databaseName = strdup(tmp);

    if (database != NULL) {
        if (strcmp(database->name, databaseName) == 0) {
            free(databaseName);
            return database;
        }
    }

    databaseFree(database);
    database = databaseOpen(databaseName);

    if (database == NULL) {
        printf("Database not found\n");
        free(databaseName);
    }

    return database;
}

void mainDescribeTable(Database* database, char* command) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    char* tableName = malloc(sizeof(char) * 100);
    sscanf(command, "describe %s", tableName);

    if (!tableExists(database, tableName)) {
        free(tableName);
        printf("Table not found\n");
        return;
    }

    Table* table = databaseGetTable(database, tableName);

    printf("%s:\n", table->name);

    for (int i = 0; i < table->lengthAttributes; i++) {
        char* type = attributTypeGet(table->attributes[i]->type);
        printf("%s : %s\n", table->attributes[i]->name, type);
        free(type);
    }

    free(tableName);
}

void mainInsertIntoTables(Database* database, char* command) {
    if (database == NULL) {
        return;
    }

    Table* table = mainGetInsertTable(database, command);
    Occurence* occurence = mainGetOccurenceInsertValues(command);

    if (!occurenceIsCorrect(table, occurence)) {
        return;
    }

    tableWriteOccurence(table, occurence);
    tableInsertOccurence(table, occurence);
}

Table* mainGetInsertTable(Database * database, char* command) {
    char* tableName = malloc(sizeof(char) * strlen(command));
    sscanf(command, "insert into %s values", tableName);

    if (!tableExists(database, tableName)) {
        free(tableName);
        return NULL;
    }

    Table* table = databaseGetTable(database, tableName);

    free(tableName);

    return table;
}

Occurence* mainGetOccurenceInsertValues(char* command) {
    char* tableName = malloc(sizeof(char) * strlen(command));
    char* values = malloc(sizeof(char) * strlen(command));

    sscanf(command, "insert into %s values(%[^)\n])", tableName, values);

    free(tableName);

    Occurence* occurence = occurenceNew();

    char* value = strtok(values, ",");

    while (value != NULL) {
        trim(value);
        AttributType type = occurenceValueGetType(value);
        occurenceAdd(occurence, occurenceValueNew(value, type));
        value = strtok(NULL, ",");
    }

    return occurence;
}

void mainDeleteFromTable(Database* database, char* command) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    Table* table = mainGetDeleteTable(database, command);

    if (table == NULL) {
        printf("Table doesn't exist\n");
        return;
    }

    int nbOccurencesRes;
    Occurence** ocs = mainGetOccurencesFromJointures(table, command, &nbOccurencesRes);

    for (int i = 0; i < nbOccurencesRes; i++) {
        tableRemoveOccurence(table, ocs[i]);
    }

    tableWriteModifications(database, table);
}

Table* mainGetDeleteTable(Database* database, char* command) {
    char* tableName = malloc(sizeof(char) * (strlen(command) + 1));
    sscanf(command, "delete from %s", tableName);

    if (!tableExists(database, tableName)) {
        free(tableName);
        return NULL;
    }

    Table* table = databaseGetTable(database, tableName);

    free(tableName);

    return table;
}

Occurence** mainGetOccurencesFromJointures(Table* table, char* command, int* nbOccurencesRes) {
    if (strstr(command, "where") == NULL) {
        *nbOccurencesRes = table->lengthOccurences;
        Occurence** ocs = malloc(sizeof(Occurence*) * *nbOccurencesRes);
        for (int i = 0; i < *nbOccurencesRes; i++) {
            ocs[i] = table->occurences[i];
        }
        return ocs;
    }

    char* conditionsS = malloc(sizeof(char) * strlen(command));
    sscanf(command, "delete from %*s where %[^\n]", conditionsS);

    char **conditionsListString = malloc(sizeof(char*) * 10);
    int nbConditions = 0;

    conditionsListString[nbConditions] = malloc(sizeof(char) * 100);
    strcpy(conditionsListString[nbConditions], "");

    char* token = strtok(conditionsS, " ");

    while (token != NULL) {
        if (strcmp(token, "and") != 0) {
            strcat(conditionsListString[nbConditions], token);
            strcat(conditionsListString[nbConditions], " ");
            token = strtok(NULL, " ");
        } else {
            trim(conditionsListString[nbConditions]);
            nbConditions++;
            conditionsListString[nbConditions] = malloc(sizeof(char) * 100);
            strcpy(conditionsListString[nbConditions], "");
            token = strtok(NULL, " ");
        }
    }

    trim(conditionsListString[nbConditions]);
    nbConditions++;

    Condition** conditions = malloc(sizeof(Condition) * nbConditions);

    for (int i = 0; i < nbConditions; i++) {
        conditions[i] = conditionGet(conditionsListString[i]);
    }

    int correctConditions = conditionAreCorrects(table, conditions, nbConditions);

    if (!correctConditions) {
        return NULL;
    }

    Occurence** ocs = getAllOccurencesFromConditions(table, conditions, nbConditions, nbOccurencesRes);

    return ocs;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "partie2.h"

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
                printf("Delete\n");
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
    }

    return database;
}

void mainDescribeTable(Database* database, char* command) {
    if (database == NULL) {
        return;
    }

    char* tableName = malloc(sizeof(char) * 100);
    sscanf(command, "describe %s", tableName);

    if (!tableExists(database, tableName)) {
        free(tableName);
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
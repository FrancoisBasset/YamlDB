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
            case Use:
                databaseFree(database);
                database = mainUseDatabase(command);
                show(database);
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
                printf("Describe");
                break;
            case Help:
                printf("Help\n");
                break;
            case Exit:
                printf("Exit\n");
                break;
            case Unknown:
                printf("Unknown\n");
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

Database* mainUseDatabase(char* command) {
    char* tmp = strtok(command, " ");

    for (int i = 1; i < 2; i++) {
        tmp = strtok(NULL, " ");
    }

    char* databaseName = malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(databaseName, tmp);

    Database* database = databaseOpen(databaseName);

    if (database == NULL) {
        printf("Database not found\n");
    }

    return database;
}

void show(Database* d) {
    if (d == NULL) {
        return;
    }

    printf("%s\n", d->name);
    printf("%d / %d tables\n\n", d->lengthTables, d->capacityTables);

    for (int i = 0; i < d->lengthTables; i++) {
        printf("\n%s\n", d->tables[i]->name);

        for (int j = 0; j < d->tables[i]->lengthAttributes; j++) {
            char* type = attributTypeGet(d->tables[i]->attributes[j]->type);
            printf("    - %s   %s\n", d->tables[i]->attributes[j]->name, type);
            free(type);
        }
    }
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

    tableInsertOccurence(table, occurence);
}

Table* mainGetInsertTable(Database * database, char* command) {
    char* tableName = malloc(sizeof(char) * strlen(command));
    sscanf(command, "insert into %s values", tableName);

    if (!tableExists(database, tableName)) {
        free(tableName);
        return NULL;
    }

    Table* table = NULL;

    for (int i = 0; i < database->lengthTables; i++) {
        if (strcmp(database->tables[i]->name, tableName) == 0) {
            return database->tables[i];
        }
    }

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
        //Stocker le attr type dans la donnée membres pour savoir si c'est du bon type
        occurenceAdd(occurence, occurenceValueNew(value, type));
        value = strtok(NULL, ",");
    }

    return occurence;
}
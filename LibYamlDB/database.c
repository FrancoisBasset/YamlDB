#include <stdlib.h>
#include <string.h>

#include "database.h"

/*
 * Create .yaml file and return a Database
 */
Database* newDatabase(char* name) {
    if (name == NULL) {
        return NULL;
    }

    int sizeFileTitle = (int) strlen(name) + 5;

    char* fileName = malloc(sizeof(char) * sizeFileTitle);

    strcpy(fileName, name);
    strcpy(fileName, ".yaml");

    FILE* fileCheck = fopen(fileName, "r");

    if (fileCheck != NULL) {
        fclose(fileCheck);
        free(fileName);
        return NULL;
    }

    Database* database = malloc(sizeof(Database));
    database->lengthTables = 0;
    database->file = fopen(fileName, "w");

    if (!writeNewDatabase(database)) {
        return NULL;
    }

    return database;
}

int writeNewDatabase(Database* database) {
    if (database == NULL) {
        return 0;
    }

    fputs("    tables:\n", database->file);

    return 1;
}

Database* openDatabase(char* name) {
    if (name == NULL) {
        return NULL;
    }

    int sizeFileName = (int) strlen(name) + 5;

    char *fileName = malloc(sizeof(char) * sizeFileName);

    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        free(fileName);
        return NULL;
    }

    Database* database = malloc(sizeof(Database));

    database->file = file;

    return database;
}

int freeDatabase(Database* database) {
    if (database == NULL) {
        return 0;
    }

    fclose(database->file);

    int i;

    for (i = 0; i < database->lengthTables; i++) {
        //freeTable(database->tables[i]);
    }

    free(database);

    return 1;
}
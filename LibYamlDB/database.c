#include <stdlib.h>
#include <string.h>

#include "database.h"

Database* newDatabase(char* name) {
    if (name == NULL) {
        return NULL;
    }

    int sizeFileTitle = strlen(name) + 5;

    char* fileName = malloc(sizeof(char) * sizeFileTitle);

    strcpy(fileName, name);
    strcpy(fileName, ".yaml");

    FILE* fileCheck = fopen(fileName, "r");

    if (fileCheck != NULL) {
        fclose(fileCheck);
        free(fileName);
        return  NULL;
    }

    Database* database = malloc(sizeof(Database));
    database->lengthTables = 0;
    database->file = fopen(fileName, "w");

    writeNewDatabase(database);
}

int writeNewDatabase(Database* database) {
    if (database == NULL) {
        return -1;
    }

    fputs("    tables:\n", database->file);

    return 0;
}

int freeDatabase(Database* database) {
    if (database == NULL) {
        return -1;
    }

    fclose(database->file);

    int i;

    for (i = 0; i < database->lengthTables; i++) {
        freeTable(database->tables[i]);
    }

    free(database);

    return 0;
}
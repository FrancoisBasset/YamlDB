#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "database.h"

/*
 * Create .yaml file and return a Database
 */
Database* databaseNew(char* name) {
    if (name == NULL) {
        return NULL;
    }

    char* fileName = malloc(sizeof(char) * (strlen(name) + 6));
    sprintf(fileName, "%s.yaml", name);

    FILE* fileCheck = fopen(fileName, "r");

    if (fileCheck != NULL) {
        fclose(fileCheck);
        free(fileName);
        return NULL;
    }

    Database* database = malloc(sizeof(Database));

    database->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(database->name, name);

    database->lengthTables = 0;
    database->capacityTables = 5;
    database->tables = malloc(sizeof(Table) * database->capacityTables);

    if (!databaseWriteNew(database, fileName)) {
        free(fileName);
        return NULL;
    }

    free(fileName);

    return database;
}

/*
 * Write a database file
 */
int databaseWriteNew(Database* database, char* fileName) {
    if (database == NULL) {
        return 0;
    }

    database->file = fopen(fileName, "w");

    if (database->file == NULL) {
        return 0;
    }

    fputs("tables:\n", database->file);

    return 1;
}

/*
 * Return an existing database
 */
Database* databaseOpen(char* name) {
    if (name == NULL) {
        return NULL;
    }

    char*  fileName = malloc(sizeof(char) * (strlen(name) + 6));
    sprintf(fileName, "%s.yaml", name);

    FILE* file = fopen(fileName, "r");
    free(fileName);

    if (file == NULL) {
        return NULL;
    }

    char* firstLine = malloc(sizeof(char) * 8);
    strcpy(firstLine, "");
    fgets(firstLine, 8, file);

    if (strcmp(firstLine, "tables:") != 0) {
        free(firstLine);
        fclose(file);
        return NULL;
    }

    free(firstLine);

    Database* database = malloc(sizeof(Database));
    database->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(database->name, name);
    database->file = file;

    if (!databaseSetTables(database)) {
        databaseFree(database);
        return NULL;
    }

    return database;
}

/*
 * Retrieve tables
 */
// todo Retrieve tables from files
int databaseSetTables(Database* database) {
    database->lengthTables = 0;
    database->capacityTables = database->lengthTables + 5;
    database->tables = malloc(sizeof(Table) * database->capacityTables);

    return 1;
}

/*
 * Free ressources of a database
 */
int databaseFree(Database* database) {
    if (database == NULL) {
        return 0;
    }

    free(database->name);
    fclose(database->file);

    int i;

    for (i = 0; i < database->lengthTables; i++) {
        tableFree(database->tables[i]);
    }

    free(database->tables);
    free(database);

    return 1;
}

/*
 * Delete .yaml file associated with a database
 */
void databaseDeleteFile(Database* database) {
    if (database == NULL) {
        return;
    }

    char* databaseFileName = malloc(sizeof(char) * (strlen(database->name) + 6));
    sprintf(databaseFileName, "%s.yaml", database->name);

    unlink(databaseFileName);
    free(databaseFileName);
}

int databaseDelete(Database* database) {
    if (database == NULL) {
        return 0;
    }

    databaseDeleteFile(database);

    int i;

    for (i = 0; i < database->lengthTables; i++) {
        tableDeleteFile(database, database->tables[i]);
    }

    return databaseFree(database);
}

int tableExists(Database* database, char* tableName) {
    int i;

    fseek(database->file, 12, SEEK_SET);
    char *scan = malloc(sizeof(char) * 20);

    for (i = 0; i < database->lengthTables; i++) {
        fscanf(database->file, "    - %s", scan);

        if (strcmp(scan, tableName) == 0) {
            return 1;
        }
    }

    return 0;
}

// todo Voir si une table existe déjà
void databaseAddNewTable(Database* database, Table* table) {
    if (database->lengthTables == database->capacityTables) {
        database->capacityTables += 5;

        Table** tables = malloc(sizeof(Table) * database->capacityTables);

        int i;

        for (i = 0; i < database->lengthTables; i++) {
            tables[i] = database->tables[i];
        }

        database->tables = tables;
    }

    Table* newTable = malloc(sizeof(Table));
    newTable->name = malloc(sizeof(char) * (strlen(table->name) + 1));
    strcpy(newTable->name, table->name);

    newTable->file = table->file;

    newTable->lengthAttributes = table->lengthAttributes;
    newTable->attributes = malloc(sizeof(Attribut) * newTable->lengthAttributes);

    int i;

    for (i = 0; i < newTable->lengthAttributes; i++) {
        newTable->attributes[i] = malloc(sizeof(Attribut));
        newTable->attributes[i]->name = malloc(sizeof(char) * (strlen(table->attributes[i]->name) + 1));
        strcpy(newTable->attributes[i]->name, table->attributes[i]->name);
        newTable->attributes[i]->type = table->attributes[i]->type;
    }

    database->tables[database->lengthTables] = newTable;
    database->lengthTables++;

    char *line = malloc(sizeof(char) * (strlen(table->name) + 8));
    sprintf(line, "    - %s\n", table->name);

    fputs(line, database->file);
    free(line);

    tableWriteAttributes(table);
    fputs("\noccurences:\n", table->file);
}
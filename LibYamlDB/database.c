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

    database->name = name;
    database->lengthTables = 0;
    database->capacityTables = 5;
    database->tables = malloc(sizeof(Table) * database->capacityTables);

    if (!databaseWriteNew(database, fileName)) {
        free(fileName);
        databaseFree(database);
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

    FILE* file = fopen(fileName, "r+");
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
    database->name = name;
    database->file = file;

    if (!databaseRetrieveTables(database)) {
        databaseFree(database);
        return NULL;
    }

    return database;
}

/*
 * Retrieve tables
 */
int databaseRetrieveTables(Database* database) {
    fseek(database->file, 0, SEEK_END);
    long end = ftell(database->file);

    if (end == 8) {
        database->lengthTables = 0;
        database->capacityTables = database->lengthTables + 5;
        database->tables = malloc(sizeof(Table) * database->capacityTables);
        return 1;
    }

    fseek(database->file, 8, SEEK_SET);

    char* line = malloc(sizeof(char) * 20);
    strcpy(line, "");

    int i = 0;

    while (end != ftell(database->file)) {
        fscanf(database->file, "    - %s\n", line);
        i++;
    }

    printf("i = %d", i);

    database->lengthTables = i;
    database->capacityTables = i + 5;
    database->tables = malloc(sizeof(Table) * database->capacityTables);

    fseek(database->file, 8, SEEK_SET);

    i = 0;
    while (end != ftell(database->file)) {
        fscanf(database->file, "    - %s\n", line);
        database->tables[i] = tableOpen(database->name, line);
        i++;
    }

    free(line);

    return 1;
}

/*
 * Free ressources of a database
 */
int databaseFree(Database* database) {
    if (database == NULL) {
        return 0;
    }

    int i;

    for (i = 0; i < database->lengthTables; i++) {
        tableFree(database->tables[i]);
    }

    free(database->name);
    fclose(database->file);
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

    for (i = 0; i < database->lengthTables; i++) {
        if (strcmp(database->tables[i]->name, tableName) == 0) {
            return 1;
        }
    }

    return 0;
}

void databaseAddNewTable(Database* database, Table* table) {
    if (database->lengthTables == database->capacityTables) {
        database->capacityTables += 5;

        Table** tables = malloc(sizeof(Table) * database->capacityTables);

        int i;

        for (i = 0; i < database->lengthTables; i++) {
            tables[i] = database->tables[i];
        }

        free(database->tables);

        database->tables = tables;
    }

    database->tables[database->lengthTables] = table;
    database->lengthTables++;

    char *line = malloc(sizeof(char) * (strlen(table->name) + 8));
    sprintf(line, "    - %s\n", table->name);

    fputs(line, database->file);
    free(line);

    tableWriteAttributes(table);
    fputs("\noccurences:\n", table->file);
}

// todo remove table entry from database file
void databaseDeleteTable(Database* database, char* tableName) {
    if (!tableExists(database, tableName)) {
        return;
    }

    int i;

    for (i = 0; i < database->lengthTables; i++) {
        if (strcmp(database->tables[i]->name, tableName) == 0) {
            break;
        }
        i++;
    }

    tableDelete(database, database->tables[i]);
}
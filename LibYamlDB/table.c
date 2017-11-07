#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "table.h"
#include "attributType.h"

/*
 * Create new table
 */
Table* tableNew(char* databaseName, char* name, int lengthAttributes, Attribut** attributes) {
    if (name == NULL) {
        return NULL;
    }

    char* fileName = malloc(sizeof(char) * (strlen(databaseName) + (strlen(name) + 7)));
    sprintf(fileName, "%s.%s.yaml", databaseName, name);

    FILE *file = fopen(fileName, "r");

    if (file != NULL) {
        free(fileName);
        return NULL;
    }

    Table* table = malloc(sizeof(Table));

    table->file = fopen(fileName, "w");
    free(fileName);

    table->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(table->name, name);

    table->attributes = malloc(sizeof(Attribut) * lengthAttributes);
    table->lengthAttributes = lengthAttributes;

    int i;

    for (i = 0; i < lengthAttributes; i++) {
        table->attributes[i] = malloc(sizeof(Attribut));
        table->attributes[i]->name = malloc(sizeof(char) * (strlen(attributes[i]->name) + 1));
        strcpy(table->attributes[i]->name, attributes[i]->name);
        table->attributes[i]->type = attributes[i]->type;
    }

    return table;
}

// todo tableOpen

Table* tableOpen(char* databaseName, char* tableName) {
    char* fileName = malloc(sizeof(char) * (strlen(databaseName) + strlen(tableName) + 7));
    sprintf(fileName, "%s.%s.yaml", databaseName, tableName);

    FILE* file = fopen(fileName, "r");
    free(fileName);

    if (file == NULL) {
        return NULL;
    }

    Table* table = malloc(sizeof(Table));

    table->file = file;
    table->name = malloc(sizeof(char) * (strlen(tableName) + 1));
    strcpy(table->name, tableName);

    fseek(table->file, 12, SEEK_SET);

    char* attrLine = malloc(sizeof(char) * 50);
    strcpy(attrLine, " ");

    int i = -1;
    while (strcmp(attrLine, "\n") != 0) {
        fgets(attrLine, 50, table->file);
        i++;
    }

    table->lengthAttributes = i;
    table->attributes = malloc(sizeof(Attribut) * i);

    fseek(table->file, 12, SEEK_SET);

    char* attrName = malloc(sizeof(char) * 50);
    int type = 0;

    for (i = 0; i < table->lengthAttributes; i++) {
        table->attributes[i] = malloc(sizeof(Attribut));
        fgets(attrLine, 50, table->file);
        //printf("%s", attrLine);
        sscanf(attrLine, "    %[^:]: %d", attrName, &type);

        printf("%s %d", attrName, type);

        table->attributes[i]->name = malloc(sizeof(char) * (strlen(attrName) + 1));
        strcpy(table->attributes[i]->name, attrName);
        switch (type) {
            case 1:
                table->attributes[i]->type = Char;
                break;
            case 2:
                table->attributes[i]->type = Int;
                break;
            case 3:
                table->attributes[i]->type = Double;
                break;
            case 4:
                table->attributes[i]->type = String;
                break;
        }
    }

    free(attrLine);
    free(attrName);

    return table;
}

/*
 * Write attributes on table file
 */
void tableWriteAttributes(Table* table) {
    fputs("attributes:\n", table->file);

    int i;

    for (i = 0; i < table->lengthAttributes; i++) {
        char* line = malloc(sizeof(char) * (strlen(table->attributes[i]->name) + 9));
        sprintf(line, "    %s: %d\n", table->attributes[i]->name, table->attributes[i]->type);
        fputs(line, table->file);
        free(line);
    }
}

/*
 * Free ressources of a table
 */
void tableFree(Table* table) {
    if (table == NULL) {
        return;
    }

    int i;

    for (i = 0; i < table->lengthAttributes; i++) {
        free(table->attributes[i]->name);
        free(table->attributes[i]);
    }

    free(table->name);
    fclose(table->file);
    free(table->attributes);
    free(table);
}

/*
 * Delete file belongs to a table
 */
void tableDeleteFile(Database* database, Table* table) {
    char* tableFileName = malloc(sizeof(char) * (strlen(table->name) + 6));
    sprintf(tableFileName, "%s.%s.yaml", database->name, table->name);

    unlink(tableFileName);
    free(tableFileName);
}


void tableDelete(Database* database, Table* table) {
    tableDeleteFile(database, table);

    int tablePosition = 0;

    while (strcmp(table->name, database->tables[tablePosition]->name) != 0) {
        tablePosition++;
    }

    tableFree(table);
}
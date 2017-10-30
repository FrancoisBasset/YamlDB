#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "table.h"

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

    if (file == NULL) {
        free(fileName);
        return NULL;
    }

    Table* table = malloc(sizeof(Table));

    table->file = fopen(fileName, "w");
    free(fileName);

    table->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(table->name, name);

    printf("%s", table->name);

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

/*
 * Write attributes on table file
 */
void tableWriteAttributes(Table* table) {
    fputs("    tables:", table->file);

    int i;

    for (i = 0; i < table->lengthAttributes; i++) {
        char* line = malloc(sizeof(char) * (strlen(table->attributes[i]->name) + 8));
        sprintf(line, "    %s: %d", table->attributes[i]->name, table->attributes[i]->type);
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

    free(table->name);

    int i;

    for (i = 0; i < table->lengthAttributes; i++) {
        free(table->attributes[i]->name);
        free(table->attributes[i]);
    }

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
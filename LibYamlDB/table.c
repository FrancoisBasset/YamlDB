#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "table.h"
#include "attributType.h"
#include "occurence.h"

Table* tableNew(char* databaseName, char* name, int lengthAttributes, Attribut** attributes) {
    if (name == NULL) {
        return NULL;
    }

    char* fileName = malloc(sizeof(char) * (strlen(databaseName) + (strlen(name) + 7)));
    sprintf(fileName, "%s.%s.yaml", databaseName, name);

    FILE *file = fopen(fileName, "r");

    if (file != NULL) {
        fclose(file);
        free(fileName);
        return NULL;
    }

    Table* table = malloc(sizeof(Table));
    table->file = fopen(fileName, "w");
    table->name = name;
    table->attributes = attributes;
    table->lengthAttributes = lengthAttributes;

    free(fileName);

    return table;
}

Table* tableOpen(char* databaseName, char* tableName) {
    char* fileName = malloc(sizeof(char) * (strlen(databaseName) + strlen(tableName) + 7));
    sprintf(fileName, "%s.%s.yaml", databaseName, tableName);

    if (!tableIs(fileName)) {
        free(fileName);
        return NULL;
    }

    FILE* file = fopen(fileName, "r+");
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
        sscanf(attrLine, "    %[^:]: %d", attrName, &type);

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
            case 5:
                table->attributes[i]->type = IntAutoIncrement;
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

void tableWriteOccurence(Table* table, Occurence* occurence) {
    fputs("    - [", table->file);

    int i;
    char* text = malloc(sizeof(char) * 100);

    for (i = 0; i < occurence->length - 1; i++) {
        text = occurence->values[i]->value;
        sprintf(text, "%s, ", text);
        fputs(text, table->file);
    }

    i++;
    text = occurence->values[i]->value;
    sprintf(text, "%s, ", text);
    fputs(text, table->file);
}

void tableInsertOccurence(Table* table, Occurence* occurence) {
    tableWriteOccurence(table, occurence);
    // inserer une occurence dans le tableau d'occurences
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
    char* tableFileName = malloc(sizeof(char) * (strlen(database->name) + strlen(table->name) + 7));
    sprintf(tableFileName, "%s.%s.yaml", database->name, table->name);

    unlink(tableFileName);
    free(tableFileName);
}

int tableIs(char* fileName) {
    char* part = strstr(fileName, ".yaml");

    if (part == NULL) {
        return 0;
    }

    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        return 0;
    }

    char firstLine[12];
    fgets(firstLine, 12, file);

    if (strcmp(firstLine, "attributes:") != 0) {
        fclose(file);
        return 0;
    }

    fclose(file);

    return 1;
}
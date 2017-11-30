#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "table.h"

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

    table->lengthOccurences = 0;
    table->capacityOccurences = 5;
    table->occurences = malloc(sizeof(Occurence) * table->capacityOccurences);

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
    table->name = strdup(tableName);

    tableRetrieveAttributes(table);
    tableRetrieveOccurences(table);

    return table;
}

void tableRetrieveAttributes(Table* table) {
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
        fgets(attrLine, 50, table->file);
        sscanf(attrLine, "    %[^:]: %d", attrName, &type);

        table->attributes[i] = attributNew(attrName, type);
    }

    free(attrLine);
    free(attrName);
}

void tableRetrieveOccurences(Table* table) {
    char* attrLine = malloc(sizeof(char) * 50);

    fgets(attrLine, 50, table->file);
    fgets(attrLine, 50, table->file);

    long current = ftell(table->file);
    fseek(table->file, 0, SEEK_END);
    long end = ftell(table->file);
    fseek(table->file, current, SEEK_SET);

    int i = 0;

    while (ftell(table->file) != end) {
        fgets(attrLine, 50, table->file);
        i++;
    }

    table->lengthOccurences = i;
    table->capacityOccurences = i + 5;

    fseek(table->file, current, SEEK_SET);
    table->occurences = malloc(sizeof(Occurence) * table->lengthOccurences);

    for (i = 0; i < table->lengthOccurences; i++) {
        fgets(attrLine, 50, table->file);
        table->occurences[i] = occurenceOpen(attrLine);
    }

    free(attrLine);
}

void tableWriteAttributes(Table* table) {
    fputs("attributes:\n", table->file);

    int i;

    for (i = 0; i < table->lengthAttributes; i++) {
        char* line = malloc(sizeof(char) * (strlen(table->attributes[i]->name) + 9));
        sprintf(line, "    %s: %d\n", table->attributes[i]->name, table->attributes[i]->type);
        fputs(line, table->file);
        free(line);
    }

    fputs("\noccurences:", table->file);
}

void tableWriteOccurence(Table* table, Occurence* occurence) {
    fseek(table->file, 0, SEEK_END);

    fputs("    - [", table->file);

    int i = 0;
    char* text = malloc(sizeof(char) * 100);

    for (i = 0; i < occurence->length; i++) {
        text = occurence->values[i]->value;
        sprintf(text, "%s, ", text);
        fputs(text, table->file);
    }
    fputs("]\n", table->file);
}

void tableInsertOccurence(Table* table, Occurence* occurence) {
    if (table->lengthOccurences == table->capacityOccurences) {
        table->capacityOccurences += 5;
        Occurence** newOccurences = malloc(sizeof(Occurence) * table->capacityOccurences);

        for (int i = 0; i < table->lengthOccurences; i++) {
            newOccurences[i] = table->occurences[i];
        }

        free(table->occurences);
        table->occurences = newOccurences;
    }

    table->occurences[table->lengthOccurences] = occurence;
    table->lengthOccurences++;
}

void tableFree(Table* table) {
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < table->lengthAttributes; i++) {
        attributFree(table->attributes[i]);
    }

    for (int i = 0; i < table->lengthOccurences; i++) {
        occurenceFree(table->occurences[i]);
    }

    free(table->name);
    fclose(table->file);
    free(table->attributes);
    free(table->occurences);
    free(table);
}

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
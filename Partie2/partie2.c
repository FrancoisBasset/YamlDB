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
                database = mainUseDatabase(database, command);
                break;
            case Select:
                mainSelectFrom(database, command);
                break;
            case Insert:
                mainInsertIntoTables(database, command);
                break;
            case Update:
                mainUpdateTable(database, command);
                break;
            case Delete:
                mainDeleteFromTable(database, command);
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
                printf("Commands:\n");
                printf("use <database>\nshow databases\nshow tables\ndescribe <table>\nupdate <table> set <affectation> where <conditions>\n");
                printf("delete from <table> where <conditions>\nselect <attributes> where <conditions>\nhelp\nexit");
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
        printf("Database is not open\n");
        return;
    }

    for (int i = 0; i < database->lengthTables; i++) {
        printf("%s\n", database->tables[i]->name);
    }
}

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
        free(databaseName);
    }

    return database;
}

void mainDescribeTable(Database* database, char* command) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    char* tableName = malloc(sizeof(char) * 100);
    sscanf(command, "describe %s", tableName);

    if (!tableExists(database, tableName)) {
        free(tableName);
        printf("Table not found\n");
        return;
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

    Table* table = mainGetTableFromCommand(database, command, Insert);
    Occurence* occurence = mainGetOccurenceInsertValues(command);

    if (!occurenceIsCorrect(table, occurence)) {
        return;
    }

    tableWriteOccurence(table, occurence);
    tableInsertOccurence(table, occurence);
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

void mainDeleteFromTable(Database* database, char* command) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    Table* table = mainGetTableFromCommand(database, command, Delete);

    if (table == NULL) {
        printf("Table doesn't exist\n");
        return;
    }

    int nbOccurencesRes;
    Occurence** ocs = mainGetOccurencesFromJointures(table, command, &nbOccurencesRes);

    if (ocs == NULL) {
        return;
    }

    for (int i = 0; i < nbOccurencesRes; i++) {
        tableRemoveOccurence(table, ocs[i]);
    }

    free(ocs);

    tableWriteModifications(database, table);
}

Occurence** mainGetOccurencesFromJointures(Table* table, char* command, int* nbOccurencesRes) {
    if (strstr(command, "where") == NULL) {
        *nbOccurencesRes = table->lengthOccurences;
        Occurence** ocs = malloc(sizeof(Occurence*) * *nbOccurencesRes);
        for (int i = 0; i < *nbOccurencesRes; i++) {
            ocs[i] = table->occurences[i];
        }
        return ocs;
    }

    char* conditionsS = strstr(command, "where");
    sscanf(conditionsS, "where %[^\n]", conditionsS);

    char **conditionsListString = malloc(sizeof(char*) * 10);
    int nbConditions = 0;

    conditionsListString[nbConditions] = malloc(sizeof(char) * 100);
    strcpy(conditionsListString[nbConditions], "");

    char* token = strtok(conditionsS, " ");

    while (token != NULL) {
        if (strcmp(token, "and") != 0) {
            strcat(conditionsListString[nbConditions], token);
            strcat(conditionsListString[nbConditions], " ");
            token = strtok(NULL, " ");
        } else {
            trim(conditionsListString[nbConditions]);
            nbConditions++;
            conditionsListString[nbConditions] = malloc(sizeof(char) * 100);
            strcpy(conditionsListString[nbConditions], "");
            token = strtok(NULL, " ");
        }
    }

    trim(conditionsListString[nbConditions]);
    nbConditions++;

    Condition** conditions = malloc(sizeof(Condition) * nbConditions);

    for (int i = 0; i < nbConditions; i++) {
        conditions[i] = conditionGet(conditionsListString[i]);
    }

    int correctConditions = conditionAreCorrects(table, conditions, nbConditions);

    Occurence** ocs = NULL;

    if (correctConditions) {
        ocs = getAllOccurencesFromConditions(table, conditions, nbConditions, nbOccurencesRes);
    }

    for (int i = 0; i < nbConditions; i++) {
        free(conditions[i]);
        free(conditionsListString[i]);
    }
    free(conditionsListString);
    free(conditions);

    if (!correctConditions) {
        printf("Incorrect conditions\n");
        return NULL;
    }

    return ocs;
}

Affectation** mainGetAllAffectationsFromUpdate(Table* table, char* command, int* nbAffectationRes) {
    char* affectationLine = malloc(sizeof(char) * 1000);
    char* line = strdup(command);
    line = strstr(line, "set");

    if (line == NULL) {
        free(affectationLine);
        return NULL;
    }

    line = strtok(line, " ");
    line = strtok(NULL, " ");

    strcpy(affectationLine, "");

    while (line != NULL) {
        if (strcmp(line, "where") == 0) {
            break;
        }

        strcat(affectationLine, line);
        line = strtok(NULL, " ");
    }

    *nbAffectationRes = 0;
    int correctAffectations = 1;

    line = strtok(affectationLine, ",");
    while (line != NULL) {
        (*nbAffectationRes)++;
        if (!affectationIsCorrect(table, line)) {
            correctAffectations = 0;
            break;
        }
        line = strtok(NULL, ",");
    }

    Affectation** affectations = NULL;

    if (correctAffectations) {
        affectations = malloc(sizeof(Affectation) * *nbAffectationRes);
        line = strtok(affectationLine, ",");

        int i = 0;
        while (line != NULL) {
            affectations[i] = affectationGet(line);
            i++;
            line = strtok(NULL, ",");
        }
    }

    free(affectationLine);
    free(line);

    return affectations;
}

void mainUpdateTable(Database* database, char* command) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    Table* table = mainGetTableFromCommand(database, command, Update);

    if (table == NULL) {
        printf("Table doesn't exist\n");
        return;
    }

    int nbAffectationsRes = 0;
    Affectation** affectations = mainGetAllAffectationsFromUpdate(table, command, &nbAffectationsRes);

    if (affectations == NULL) {
        printf("Set is not correct\n");
        return;
    }

    int nbOccurencesRes = 0;
    Occurence** occurences = mainGetOccurencesFromJointures(table, command, &nbOccurencesRes);

    if (occurences == NULL) {
        return;
    }

    affectationApply(table, occurences, nbOccurencesRes, affectations, nbAffectationsRes);
    tableWriteModifications(database, table);

    for (int i = 0; i < nbAffectationsRes; i++) {
        affectationFree(affectations[i]);
    }
    free(affectations);
    free(occurences);
}

Table* mainGetTableFromCommand(Database* database, char* command, CommandType commandType) {
    char* tableName = malloc(sizeof(char) * (strlen(command) + 1));

    switch (commandType) {
        case Insert:
            sscanf(command, "insert into %s values", tableName);
            break;
        case Delete:
            sscanf(command, "delete from %s", tableName);
            break;
        case Update:
            sscanf(command, "update %s", tableName);
            break;
        case Select:
            tableName = strdup(strstr(command, "from"));
            sscanf(tableName, "from %s", tableName);
            break;
    }

    if (!tableExists(database, tableName)) {
        free(tableName);
        return NULL;
    }

    Table* table = databaseGetTable(database, tableName);

    free(tableName);

    return table;
}

void mainSelectFrom(Database* database, char* command) {
    if (database == NULL) {
        printf("Database is not open\n");
        return;
    }

    Table* table = mainGetTableFromCommand(database, command, Select);

    if (table == NULL) {
        printf("Table doesn't exist");
        return;
    }

    int nbOccurencesRes = 0;
    Occurence** occurences = mainGetOccurencesFromJointures(table, command, &nbOccurencesRes);

    if (occurences == NULL) {
        return;
    }

    int nbSelectors = 0;
    char** selectors = mainGetSelectAttributes(table, command, &nbSelectors);

    for (int i = 0; i < nbSelectors; i++) {
        for (int j = 0; j < table->lengthAttributes; j++) {
            if (strcmp(selectors[i], table->attributes[j]->name) == 0) {
                printf("%20s", table->attributes[j]->name);
                break;
            }
        }
    }
    printf("\n\n");

    for (int o = 0; o < nbOccurencesRes; o++) {
        for (int v = 0; v < occurences[o]->length; v++) {
            for(int s = 0; s < nbSelectors; s++) {
                if (strcmp(table->attributes[v]->name, selectors[s]) == 0) {
                    printf("%20s", occurences[o]->values[v]->value);
                    break;
                }
            }
        }
        printf("\n");
    }
}

char** mainGetSelectAttributes(Table* table, char* command, int* nbSelectors) {
    char* selectors = strdup(command);
    sscanf(selectors, "select %s from %*s", selectors);

    if (strcmp(selectors, "*") == 0) {
        char** result = malloc(sizeof(char*) * table->lengthAttributes);
        for (int i = 0; i < table->lengthAttributes; i++) {
            result[i] = table->attributes[i]->name;
        }
        *nbSelectors = table->lengthAttributes;
        return result;
    }

    char** result = malloc(sizeof(char*) * table->lengthAttributes);
    char* selector = strtok(selectors, ", ");

    *nbSelectors = 0;
    int i = 0;
    while (selector != NULL) {
        result[i] = strdup(selector);
        selector = strtok(NULL, ", ");
        (*nbSelectors)++;
        i++;
    }

    return result;
}
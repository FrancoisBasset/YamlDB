#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "commandType.h"

CommandType getCommandType(char* command) {
    char* copy = strdup(command);
    CommandType type = Unknown;

    char* firstWord = strtok(copy, " ");
    char* secondWord = strtok(NULL, " ");

    if (strcmp(firstWord, "create") == 0) {
        if (strcmp(secondWord, "database") == 0) {
            type = CreateDatabase;
        }

        if (strcmp(secondWord, "table") == 0) {
            type = CreateTable;
        }
    }

    if (strcmp(firstWord, "drop") == 0) {
        if (strcmp(secondWord, "database") == 0) {
            type = DropDatabase;
        }

        if (strcmp(secondWord, "table") == 0) {
            type = DropTable;
        }
    }

    if (strcmp(firstWord, "use") == 0) {
        type = Use;
    }

    if (strcmp(firstWord, "select") == 0) {
        type = Select;
    }

    if (strcmp(firstWord, "insert") == 0) {
        if (strcmp(secondWord, "into") == 0) {
            type = Insert;
        }
    }

    if (strcmp(firstWord, "update") == 0) {
        type = Update;
    }

    if (strcmp(firstWord, "delete") == 0) {
        if (strcmp(secondWord, "from") == 0) {
            type = Delete;
        }
    }

    if (strcmp(firstWord, "help") == 0) {
        type = Help;
    }

    if (strcmp(firstWord, "exit") == 0) {
        type = Exit;
    }

    if (strcmp(firstWord, "\n") == 0) {
        type = Null;
    }

    if (strcmp(command, "show databases") == 0) {
        type = ShowDatabases;
    }

    if (strcmp(command, "show tables") == 0) {
        type = ShowTables;
    }

    if (strcmp(firstWord, "describe") == 0) {
        type = Describe;
    }

    free(copy);
    return type;
}
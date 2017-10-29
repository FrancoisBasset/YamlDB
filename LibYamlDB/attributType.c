#include <stdlib.h>
#include <string.h>

#include "attributType.h"

char* attributTypeGet(AttributType type) {
    char* alphaType;

    switch (type) {
        case Char:
            alphaType = malloc(sizeof(char) * 5);
            strcpy(alphaType, "char");
            break;
        case Int:
            alphaType = malloc(sizeof(char) * 4);
            strcpy(alphaType, "int");
            break;
        case Double:
            alphaType = malloc(sizeof(char) * 7);
            strcpy(alphaType, "double");
            break;
        case String:
            alphaType = malloc(sizeof(char) * 7);
            strcpy(alphaType, "String");
            break;
        default:
            alphaType = malloc(sizeof(char) * 5);
            strcpy(alphaType, "NULL");
    }

    return alphaType;
}
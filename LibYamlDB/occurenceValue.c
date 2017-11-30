#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "occurenceValue.h"

OccurenceValue* occurenceValueNew(char* value, AttributType type) {
    OccurenceValue* occurence = malloc(sizeof(OccurenceValue));
    occurence->value = value;
    occurence->type = type;

    return occurence;
}

AttributType occurenceValueGetType(char* value) {
    int lastIndex = (int)strlen(value) - 1;

    if (value[0] == '\'' && value[lastIndex] == '\'') {
        return Char;
    }

    if (value[0] == '"' && value[lastIndex] == '"') {
        return String;
    }

    if(strchr(value, '.') != NULL) {
        return Double;
    }

    return Int;
}
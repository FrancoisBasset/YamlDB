#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "occurence.h"
#include "table.h"

Occurence* occurenceNew() {
    Occurence* occurence = malloc(sizeof(Occurence));
    occurence->length = 0;
    occurence->capacity = 5;
    occurence->values = malloc(sizeof(OccurenceValue) * occurence->capacity);

    return occurence;
}

void occurenceAdd(Occurence* occurence, OccurenceValue* value) {
    if (occurence->length == occurence->capacity) {
        OccurenceValue** new = malloc(sizeof(OccurenceValue) * (occurence->capacity + 5));

        for (int i = 0; i < occurence->length; i++) {
            new[i] = occurence->values[i];
        }

        free(occurence->values);
        occurence->values = new;
        occurence->capacity += 5;
    }

    occurence->values[occurence->length] = value;
    occurence->length++;
}

int occurenceIsCorrect(Table* table, Occurence* occurence) {
    int haveAutoIncrement = 0;

    for (int i = 0; i < table->lengthAttributes; i++) {
        if (table->attributes[i]->type == IntAutoIncrement) {
            haveAutoIncrement = 1;
            break;
        }
    }

    if (!haveAutoIncrement && table->lengthAttributes != occurence->length) {
        return 0;
    }

    if (haveAutoIncrement && occurence->length != table->lengthAttributes - 1) {
        return 0;
    }

    for (int i = 0; i < table->lengthAttributes; i++) {
        if (haveAutoIncrement && table->attributes[i]->type == IntAutoIncrement) {
            continue;
        }


        if (table->attributes[i]->type != occurence->values[i]->type) {
            return 0;
        }
    }

    return 1;
}

void occurenceFree(Occurence* occurence) {
    for (int i = 0; i < occurence->length; i++) {
        free(occurence->values[i]);
    }

    free(occurence->values);
    free(occurence);
}


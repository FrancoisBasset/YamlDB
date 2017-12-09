#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "affectation.h"

Affectation* affectationGet(char* line) {
    Affectation* aff = malloc(sizeof(Affectation));
    aff->attribut = malloc(sizeof(char) * 50);
    aff->value = malloc(sizeof(char) * 50);

    sscanf(line, "%[^=]=%[^=]", aff->attribut, aff->value);

    return aff;
}

void affectationFree(Affectation* affectation) {
    free(affectation->attribut);
    free(affectation->value);
    free(affectation);
}

int affectationIsCorrect(Table* table, char* line) {
    if (strstr(line, "=") == NULL) {
        return 0;
    }

    char attribut[50];
    char value[50];

    sscanf(line, "%[^=]=%[^=]", attribut, value);

    for (int i = 0; i < table->lengthAttributes; i++) {
        if (strcmp(table->attributes[i]->name, attribut) == 0) {
            AttributType type = occurenceValueGetType(value);

            if (table->attributes[i]->type == IntAutoIncrement) {
                return 0;
            }
            if (type == table->attributes[i]->type) {
                return 1;
            }
        }
    }

    return 0;
}

void affectationApply(Table* table, Occurence** occurences, int occurencesLength, Affectation** affectations, int affectationsLength) {
    for (int a = 0; a < affectationsLength; a++) {
        for (int o = 0; o < occurencesLength; o++) {
            for (int v = 0; v < occurences[o]->length; v++) {
                if (strcmp(table->attributes[v]->name, affectations[a]->attribut) == 0) {
                    free(occurences[o]->values[v]->value);
                    occurences[o]->values[v]->value = strdup(affectations[a]->value);
                }
            }
        }
    }
}
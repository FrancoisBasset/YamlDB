#ifndef LIBYAMLDB_AFFECTATION
#define LIBYAMLDB_AFFECTATION

#include "table.h"

typedef struct Affectation {
    char* attribut;
    char* value;
} Affectation;

Affectation* affectationGet(char* line);
void affectationFree(Affectation* affectation);
int affectationIsCorrect(Table* table, char* line);
void affectationApply(Table* table, Occurence** occurences, int occurencesLength, Affectation** affectations, int affectationsLength);

#endif
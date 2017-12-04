#ifndef LIBYAMLDB_CONDITION
#define LIBYAMLDB_CONDITION

#include "attribut.h"
#include "occurence.h"
#include "conditionType.h"

typedef struct Condition {
    char* attribut;
    ConditionType type;
    char* value;
    Occurence** occurences;
    int correct;
} Condition;

Condition* conditionGet(char* line);
int conditionAreCorrects(Table* table, Condition** conditions, int nbConditions);
Occurence** getAllOccurencesFromConditions(Table* table, Condition** conditions, int nbConditions, int* nbOccurencesRes);

#endif
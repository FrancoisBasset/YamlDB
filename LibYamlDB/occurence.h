#include "occurenceValue.h"
#include "table.h"

#ifndef LIBYAMLDB_OCCURENCE_H
#define LIBYAMLDB_OCCURENCE_H

typedef struct Occurence {
    OccurenceValue** values;
    int length;
    int capacity;
} Occurence;

Occurence* occurenceNew();
void occurenceAdd(Occurence* occurence, OccurenceValue* value);

int occurenceIsCorrect(Table* table, Occurence* occurence);

void occurenceFree(Occurence* occurence);

#endif
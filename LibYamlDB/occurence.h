#ifndef LIBYAMLDB_OCCURENCE
#define LIBYAMLDB_OCCURENCE

#include "occurenceValue.h"

typedef struct Occurence {
    OccurenceValue** values;
    int length;
    int capacity;
} Occurence;

#include "table.h"

Occurence* occurenceNew();
Occurence* occurenceOpen(char* line);
void occurenceAdd(Occurence* occurence, OccurenceValue* value);

int occurenceIsCorrect(Table* table, Occurence* occurence);

void occurenceFree(Occurence* occurence);

#endif
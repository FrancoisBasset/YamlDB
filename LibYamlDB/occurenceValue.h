#ifndef LIBYAMLDB_OCCURENCEVALUE
#define LIBYAMLDB_OCCURENCEVALUE

#include "attributType.h"

typedef struct OccurenceValue {
    char* value;
    AttributType type;
} OccurenceValue;

OccurenceValue* occurenceValueNew(char* value, AttributType type);
AttributType occurenceValueGetType(char* value);

#endif
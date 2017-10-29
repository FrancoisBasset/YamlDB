#ifndef LIBYAMLDB_ATTRIBUT
#define LIBYAMLDB_ATTRIBUT

#include "attributType.h"

typedef struct Attribut {
    char* name;
    AttributType type;
} Attribut;

Attribut* attributNew(char* name, enum AttributType type);

#endif
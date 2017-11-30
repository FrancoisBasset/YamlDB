#ifndef LIBYAMLDB_ATTRIBUT
#define LIBYAMLDB_ATTRIBUT

#include "attributType.h"

typedef struct Attribut {
    char* name;
    AttributType type;
} Attribut;

Attribut* attributNew(char* name, int type);
void attributFree(Attribut* attribut);

#endif
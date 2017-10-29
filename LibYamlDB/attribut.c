#include <stdlib.h>
#include <string.h>

#include "attribut.h"

Attribut* attributNew(char* name, AttributType type) {
    Attribut* attribut = malloc(sizeof(Attribut));

    int sizeName = strlen(name) + 1;

    attribut->name = malloc(sizeof(char) * sizeName);
    strcpy(attribut->name, name);
    attribut->type = type;

    return attribut;
}
#include <stdlib.h>

#include "attribut.h"

Attribut* attributNew(char* name, AttributType type) {
    Attribut* attribut = malloc(sizeof(Attribut));

    attribut->name = name;
    attribut->type = type;

    return attribut;
}
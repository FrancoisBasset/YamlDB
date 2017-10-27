#include <stdlib.h>
#include <string.h>

#include "attribut.h"

Attribut* newAttribut(char* name, enum AttributType type) {
    Attribut* attribut = malloc(sizeof(Attribut));

    attribut->name = malloc(sizeof(char) * 2);
    strcpy(attribut->name, "r");
    attribut->type = Int;

    return attribut;
}
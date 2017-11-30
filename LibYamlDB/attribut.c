#include <stdlib.h>
#include <string.h>

#include "attribut.h"

Attribut* attributNew(char* name, int type) {
    Attribut* attribut = malloc(sizeof(Attribut));

    attribut->name = strdup(name);

    switch (type) {
        case 1:
            attribut->type = Char;
            break;
        case 2:
            attribut->type = Int;
            break;
        case 3:
            attribut->type = Double;
            break;
        case 4:
            attribut->type = String;
            break;
        case 5:
            attribut->type = IntAutoIncrement;
            break;
    }

    return attribut;
}

void attributFree(Attribut* attribut) {
    free(attribut->name);
    free(attribut);
}
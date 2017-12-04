#include <string.h>

#include "conditionType.h"

ConditionType conditionTypeGetType(char* signe) {
    if (strcmp(signe, "=") == 0) {
        return Equal;
    }

    if (strcmp(signe, "!=") == 0) {
        return NotEqual;
    }

    if (strcmp(signe, ">") == 0) {
        return Greater;
    }

    if (strcmp(signe, "<") == 0) {
        return Less;
    }

    if (strcmp(signe, ">=") == 0) {
        return GreaterEqual;
    }

    if (strcmp(signe, "<=") == 0) {
        return LessEqual;
    }

    return UnknownCondition;
}
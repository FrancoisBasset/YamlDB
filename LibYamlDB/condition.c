#include <stdlib.h>
#include <string.h>
#include "condition.h"

/*Condition* conditionNew(char* attribut, ConditionType type, char* value) {
    Condition* condition = malloc(sizeof(Condition));

    condition->attribut = attribut;
    condition->type = type;
    condition->value = value;

    return condition;
}*/

Condition* conditionGet(char* line) {
    char* attribut = strdup(strtok(line, " "));
    char* type = strdup(strtok(NULL, " "));
    char* value = strdup(strtok(NULL, ""));

    Condition* c = malloc(sizeof(Condition));
    c->attribut = attribut;
    c->type = conditionTypeGetType(type);
    c->value = value;

    return c;
}

int conditionAreCorrects(Table* table, Condition** conditions, int nbConditions) {
    for (int i = 0; i < nbConditions; i++) {
        if (conditions[i]->type == UnknownCondition) {
            return 0;
        }
    }

    int nbCorrect = 0;

    for (int c = 0; c < nbConditions; c++) {
        for (int a = 0; a < table->lengthAttributes; a++) {
            if (strcmp(conditions[c]->attribut, table->attributes[a]->name) == 0) {
                if (occurenceValueGetType(conditions[c]->value) == table->attributes[a]->type) {
                    nbCorrect++;
                    break;
                } else if (occurenceValueGetType(conditions[c]->value) == Int && table->attributes[a]->type == IntAutoIncrement) {
                    nbCorrect++;
                    break;
                }
            }
        }
    }

    return nbCorrect == nbConditions;
}

Occurence** getAllOccurencesFromConditions(Table* table, Condition** conditions, int nbConditions, int* nbOccurencesRes) {
    Occurence** occurences = malloc(sizeof(Occurence) * table->lengthOccurences);
    int index = 0;

    for (int i = 0; i < table->lengthAttributes; i++) {
        if (strcmp(table->attributes[i]->name, conditions[0]->attribut) == 0) {
            index = i;
            break;
        }
    }

    *nbOccurencesRes = 0;

    for (int o = 0; o < table->lengthOccurences; o++) {
        switch (conditions[0]->type) {
            case Equal:
                if (strcmp(table->occurences[o]->values[index]->value, conditions[0]->value) == 0) {
                    occurences[*nbOccurencesRes] = table->occurences[o];
                    (*nbOccurencesRes)++;
                }
                break;
            case NotEqual:
                if (strcmp(table->occurences[o]->values[index]->value, conditions[0]->value) != 0) {
                    occurences[*nbOccurencesRes] = table->occurences[o];
                    (*nbOccurencesRes)++;
                }
                break;
            case Greater:
                if (atof(table->occurences[o]->values[index]->value) > atof(conditions[0]->value)) {
                    occurences[*nbOccurencesRes] = table->occurences[o];
                    (*nbOccurencesRes)++;
                }
                break;
            case Less:
                if (atof(table->occurences[o]->values[index]->value) < atof(conditions[0]->value)) {
                    occurences[*nbOccurencesRes] = table->occurences[o];
                    (*nbOccurencesRes)++;
                }
                break;
            case GreaterEqual:
                if (atof(table->occurences[o]->values[index]->value) >= atof(conditions[0]->value)) {
                    occurences[*nbOccurencesRes] = table->occurences[o];
                    (*nbOccurencesRes)++;
                }
                break;
            case LessEqual:
                if (atof(table->occurences[o]->values[index]->value) <= atof(conditions[0]->value)) {
                    occurences[*nbOccurencesRes] = table->occurences[o];
                    (*nbOccurencesRes)++;
                }
                break;
        }
    }

    return occurences;
}
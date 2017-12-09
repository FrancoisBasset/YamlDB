#include <stdlib.h>
#include <string.h>
#include "condition.h"

Condition* conditionGet(char* line) {
    char* attribut = strtok(line, " ");
    char* type = strtok(NULL, " ");
    char* value = strtok(NULL, " ");

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
    int* indexes = malloc(sizeof(int) * nbConditions);

    for (int c = 0; c < nbConditions; c++) {
        for (int i = 0; i < table->lengthAttributes; i++) {
            if (strcmp(table->attributes[i]->name, conditions[c]->attribut) == 0) {
                indexes[c] = i;
                break;
            }
        }
    }

    *nbOccurencesRes = 0;

    for (int o = 0; o < table->lengthOccurences; o++) {
        int correct = 1;

        for (int c = 0; c < nbConditions; c++) {
            switch (conditions[c]->type) {
                case Equal:
                    if (strcmp(table->occurences[o]->values[indexes[c]]->value, conditions[c]->value) == 0) {}
                    else {
                        correct = 0;
                        break;
                    }
                    break;
                case NotEqual:
                    if (strcmp(table->occurences[o]->values[indexes[c]]->value, conditions[c]->value) != 0) {}
                    else {
                        correct = 0;
                        break;
                    }
                    break;
                case Greater:
                    if (atof(table->occurences[o]->values[indexes[c]]->value) > atof(conditions[c]->value)) {}
                    else {
                        correct = 0;
                        break;
                    }
                    break;
                case Less:
                    if (atof(table->occurences[o]->values[indexes[c]]->value) < atof(conditions[c]->value)) {}
                    else {
                        correct = 0;
                        break;
                    }
                    break;
                case GreaterEqual:
                    if (atof(table->occurences[o]->values[indexes[c]]->value) >= atof(conditions[c]->value)) {}
                    else {
                        correct = 0;
                        break;
                    }
                    break;
                case LessEqual:
                    if (atof(table->occurences[o]->values[indexes[c]]->value) <= atof(conditions[c]->value)) {}
                    else {
                        correct = 0;
                        break;
                    }
                    break;
            }

            if (!correct) {
                break;
            }

        }

        if (correct) {
            occurences[*nbOccurencesRes] = table->occurences[o];
            (*nbOccurencesRes)++;
        }
    }

    free(indexes);

    return occurences;
}
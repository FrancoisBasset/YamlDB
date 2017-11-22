#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabValue.h"

TabValue* tabValueNew() {
    TabValue* values = malloc(sizeof(TabValue));

    values->value = NULL;
    values->next = NULL;
}

void tabValueAddNewValue(TabValue* values, char* value) {
    if (values == NULL) {
        return;
    }

    TabValue* last = values;

    if (last->value == NULL) {
        last->value = malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(last->value, value);
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = malloc(sizeof(TabValue));
    last = last->next;

    last->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(last->value, value);
    last->next = NULL;
}

void tabValuePrint(TabValue* values) {
    TabValue* tmp = malloc(sizeof(TabValue));

    tmp = values;

    while (tmp != NULL) {
        printf("%s", tmp->value);
        tmp = tmp->next;
    }
}

void tabValueFree(TabValue* values) {
    while (values->next != NULL) {
        tabValueFree(values->next);
    }


    free(values->value);
    if (values->next != NULL) {
        free(values->next);
    }
}
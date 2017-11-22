#ifndef LIBYAMLDB_VALUE
#define LIBYAMLDB_VALUE

#include "attributType.h"

typedef struct TabValue TabValue;

struct TabValue {
    char* value;
    TabValue* next;
};

TabValue* tabValueNew();
void tabValueAddNewValue(TabValue* values, char* value);
void tabValuePrint(TabValue* values);
void tabValueFree(TabValue* values);

#endif
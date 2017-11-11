#ifndef LIBYAMLDB_ATTRIBUT_TYPE
#define LIBYAMLDB_ATTRIBUT_TYPE

typedef enum AttributType {
    Char = 1,
    Int = 2,
    Double = 3,
    String = 4,
    IntAutoIncrement = 5
} AttributType;

char* attributTypeGet(AttributType type);

#endif
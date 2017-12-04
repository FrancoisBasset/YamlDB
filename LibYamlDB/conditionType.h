#ifndef LIBYAMLDB_CONDITIONTYPE
#define LIBYAMLDB_CONDITIONTYPE

typedef enum ConditionType {
    Equal,
    NotEqual,
    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    UnknownCondition
} ConditionType;

ConditionType conditionTypeGetType(char* signe);

#endif
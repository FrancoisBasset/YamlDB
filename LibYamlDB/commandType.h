#ifndef LIBYAMLDB_COMMANDTYPE
#define LIBYAMLDB_COMMANDTYPE

typedef enum CommandType {
    Use,
    Select,
    Insert,
    Update,
    Delete,
    ShowDatabases,
    ShowTables,
    Describe,
    Help,
    Exit,
    Unknown,
    Null
} CommandType;

CommandType getCommandType(char* command);

#endif
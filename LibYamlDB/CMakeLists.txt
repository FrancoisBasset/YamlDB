cmake_minimum_required(VERSION 3.8)
project(LibYamlDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES database.c database.h table.h table.c table.h attribut.c attribut.h attributType.h attributType.c yamlUtil.c yamlUtil.h commandType.c
        commandType.h occurence.c occurence.h occurenceValue.c occurenceValue.h condition.c condition.h conditionType.h conditionType.c)
add_library(YamlDB STATIC ${SOURCE_FILES} database.c database.h table.h table.c table.h attribut.c attribut.h attributType.h attributType.c
        yamlUtil.c yamlUtil.h commandType.c commandType.h occurence.c occurence.h occurenceValue.c occurenceValue.h condition.c condition.h conditionType.h conditionType.c)
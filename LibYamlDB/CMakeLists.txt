cmake_minimum_required(VERSION 3.8)
project(LibYamlDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES database.c database.h table.h table.c table.h attribut.c attribut.h attributType.h attributType.c yamlUtil.c yamlUtil.h commandType.c commandType.h tabValue.c tabValue.h occurence.c occurence.h occurenceValue.c occurenceValue.h)
add_library(YamlDB STATIC ${SOURCE_FILES} database.c database.h table.h table.c table.h attribut.c attribut.h attributType.h attributType.c yamlUtil.c yamlUtil.h commandType.c commandType.h tabValue.c tabValue.h occurence.c occurence.h occurenceValue.c occurenceValue.h)
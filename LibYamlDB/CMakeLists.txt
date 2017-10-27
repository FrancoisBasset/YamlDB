cmake_minimum_required(VERSION 3.8)
project(LibYamlDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES database.c database.h table.h table.c table.h attribut.c attribut.h attributType.h libYamlDB.h)
add_library(YamlDB ${SOURCE_FILES} database.c database.h table.h table.c table.h attribut.c attribut.h attributType.h libYamlDB.h)
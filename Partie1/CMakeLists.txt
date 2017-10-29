cmake_minimum_required(VERSION 3.8)
project(Partie1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)

add_executable(Partie1 ${SOURCE_FILES})

find_library(YamlDB libYamlDB.a)
TARGET_LINK_LIBRARIES(Partie1 YamlDB)
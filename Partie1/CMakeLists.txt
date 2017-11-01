cmake_minimum_required(VERSION 3.8)
project(Partie1)

set(CMAKE_C_STANDARD 99)

link_directories(../LibYamlDB)

set(SOURCE_FILES main.c ../LibYamlDB/database.c ../LibYamlDB/database.h ../LibYamlDB/table.h ../LibYamlDB/table.c partie1.c partie1.h)

add_executable(Partie1 ${SOURCE_FILES} partie1.c partie1.h)

#TARGET_LINK_LIBRARIES(Partie1 YamlDB)
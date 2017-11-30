cmake_minimum_required(VERSION 3.8)
project(Partie1)

set(CMAKE_C_STANDARD 99)

link_directories(../LibYamlDB)

set(SOURCE_FILES main.c partie1.c partie1.h ../LibYamlDB/database.h ../LibYamlDB/database.c
        ../LibYamlDB/table.h ../LibYamlDB/table.c ../LibYamlDB/commandType.h ../LibYamlDB/commandType.c
        ../LibYamlDB/attributType.h ../LibYamlDB/attributType.c ../LibYamlDB/yamlUtil.h ../LibYamlDB/yamlUtil.c
        ../LibYamlDB/occurenceValue.h ../LibYamlDB/occurenceValue.c
        ../LibYamlDB/occurence.h ../LibYamlDB/occurence.c
        ../LibYamlDB/attribut.h ../LibYamlDB/attribut.c)
add_executable(Partie1 ${SOURCE_FILES} partie1.c partie1.h)
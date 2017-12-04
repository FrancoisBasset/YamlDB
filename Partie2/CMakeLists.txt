cmake_minimum_required(VERSION 3.8)
project(Partie2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c partie2.c partie2.h ../LibYamlDB/database.h ../LibYamlDB/database.c
        ../LibYamlDB/table.h ../LibYamlDB/table.c ../LibYamlDB/commandType.h ../LibYamlDB/commandType.c
        ../LibYamlDB/attributType.h ../LibYamlDB/attributType.c ../LibYamlDB/yamlUtil.h ../LibYamlDB/yamlUtil.c
        ../LibYamlDB/occurenceValue.h ../LibYamlDB/occurenceValue.c
        ../LibYamlDB/occurence.h ../LibYamlDB/occurence.c
        ../LibYamlDB/attribut.h ../LibYamlDB/attribut.c
        ../LibYamlDB/condition.h ../LibYamlDB/condition.c
        ../LibYamlDB/conditionType.h ../LibYamlDB/conditionType.c)
add_executable(Partie2 ${SOURCE_FILES} partie2.c partie2.h)
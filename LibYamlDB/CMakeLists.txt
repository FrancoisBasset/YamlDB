cmake_minimum_required(VERSION 3.8)
project(LibYamlDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES library.c library.h)
add_library(LibYamlDB ${SOURCE_FILES})
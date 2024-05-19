cmake_minimum_required(VERSION 3.20)

set(EXE_NAME cjson)
project(${EXE_NAME})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall")

set(SOURCE_FILES main.c
    src/cjson.c)

add_executable(${EXE_NAME} ${SOURCE_FILES})
target_include_directories(${EXE_NAME} PUBLIC include/)

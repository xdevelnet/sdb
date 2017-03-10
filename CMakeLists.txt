cmake_minimum_required(VERSION 3.1)
project(sdb C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
    main.c util.h rodata.h dumb_http.h)

add_executable(sdb ${SOURCE_FILES})

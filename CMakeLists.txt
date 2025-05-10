cmake_minimum_required(VERSION 3.10)
project(PASCC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")

file(GLOB_RECURSE include "include/*.h")
file(GLOB_RECURSE src "src/*.cc")

add_executable(PASCC
        ${tools}
        ${src}
        src/lex.yy.cc
        src/parser.tab.cc
        src/log.c
)
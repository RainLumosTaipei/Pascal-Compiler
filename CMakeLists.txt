cmake_minimum_required(VERSION 3.27.7)
project(Pascal-Compiler)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/**/*.cpp" "include/**/*.h")

add_executable(Pascal-Compiler
        main.cpp
        ${SOURCES}
        src/syntax/Ast.cpp
        include/syntax/Ast.h
)

target_include_directories(Pascal-Compiler PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
)

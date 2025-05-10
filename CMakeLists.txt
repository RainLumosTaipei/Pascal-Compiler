cmake_minimum_required(VERSION 3.10)
project(PASCC)

# 静态链接配置 --------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")  # 完全静态链接选项（可选）
# -------------------------------------------------------------

find_package(FLEX)
find_package(BISON)
if(FLEX_FOUND AND BISON_FOUND)
  BISON_TARGET(Parser ${CMAKE_BINARY_DIR}/../scripts/parser.y  ${CMAKE_BINARY_DIR}/../src/parser.tab.cc  DEFINES_FILE  ${CMAKE_BINARY_DIR}/../include/parser.tab.h COMPILE_FLAGS "-t")
  FLEX_TARGET(Lexer  ${CMAKE_BINARY_DIR}/../scripts/lex.l  ${CMAKE_BINARY_DIR}/../src/lex.yy.cc)
  ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories("include")
include_directories("thirdparty")

if(WIN32)
  message(STATUS "Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32 -Wno-error=register")
  file(COPY "thirdparty/clang-format.exe" DESTINATION ${CMAKE_BINARY_DIR}/../bin/)
elseif(UNIX)
  message(STATUS "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__linux__ -Wno-error=register")
  file(COPY "thirdparty/clang-format" DESTINATION ${CMAKE_BINARY_DIR}/../bin/)
endif()

file(GLOB_RECURSE include "include/*.h" "thirdparty/*.h" "thirdparty/*.hpp")
file(GLOB_RECURSE src "src/*.cc")
file(GLOB_RECURSE tools "thirdparty/*.c" "thirdparty/*.cc")

add_executable(PASCC
        ${tools}
        ${src}
        src/lex.yy.cc
        src/parser.tab.cc
        )
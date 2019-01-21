cmake_minimum_required (VERSION 3.10)
project (TRPL)
add_executable(TRPL
               main.cpp
               ast.cpp
               repl.cpp
               interpreter.cpp)

find_package(BISON)
find_package(FLEX)

include_directories(./ build/)

BISON_TARGET(Parser grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx)
FLEX_TARGET(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

add_library(parserlib ${BISON_Parser_OUTPUTS})
add_library(scannerlib ${FLEX_Scanner_OUTPUTS})

target_link_libraries(TRPL parserlib)
target_link_libraries(TRPL scannerlib)

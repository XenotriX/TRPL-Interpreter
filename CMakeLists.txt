cmake_minimum_required (VERSION 3.10)
project (TRPL)
add_executable(TRPL
               main.cpp
               ast.cpp
               )
set_target_properties(TRPL PROPERTIES CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)

include_directories(./ build/)

BISON_TARGET(Parser parser/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx)
FLEX_TARGET(Scanner parser/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

add_library(parserlib parser/parser.cpp ${BISON_Parser_OUTPUTS})
add_library(scannerlib ${FLEX_Scanner_OUTPUTS})
add_library(interpreter interpreter/interpreter.cpp)
add_library(repl repl/repl.cpp)
set_target_properties(interpreter PROPERTIES CXX_STANDARD 17)

target_link_libraries(TRPL parserlib)
target_link_libraries(TRPL scannerlib)
target_link_libraries(TRPL interpreter)
target_link_libraries(TRPL repl)

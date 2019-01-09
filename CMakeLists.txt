cmake_minimum_required (VERSION 3.10)
project (TRPL)
add_executable(TRPL
               testscanner.c)

find_package(FLEX)
FLEX_TARGET(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)
set(SRC_FILES ${FLEX_Scanner_OUTPUTS})
add_library(scannerlib ${SRC_FILES})
target_link_libraries(scannerlib crypto c ${FLEX_LIBRARIES})
target_link_libraries(TRPL scannerlib)

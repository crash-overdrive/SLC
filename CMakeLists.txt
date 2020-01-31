cmake_minimum_required(VERSION 3.10)

project(slc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror \
  -Wno-long-long -Wno-variadic-macros -fexceptions -DNDEBUG")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_target(submit
    COMMAND cp Makefile.submit Makefile
    COMMAND zip -FSr A1.zip . -x "*build*" "*.git*"
    COMMAND /u/cs_build/bin/marmoset --username=-f5fei-c39gao-spratap- submit CS444 A1Code A1.zip
    COMMAND rm Makefile A1.zip
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    VERBATIM
)

add_subdirectory(src)
add_subdirectory(test)
enable_testing()

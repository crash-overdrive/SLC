file(GLOB HEADER_FILES *.hpp)
file(GLOB SOURCES *.cpp)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(slc STATIC ${SOURCES} ${HEADER_FILES})
target_include_directories(slc PRIVATE .)

add_executable(joosc main.cpp)
target_link_libraries(joosc slc)

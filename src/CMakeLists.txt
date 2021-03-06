configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp)

file(GLOB HEADER_FILES *.hpp)
file(GLOB SOURCES *.cpp)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)

find_package(Python3 COMPONENTS Interpreter)

set(CFG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/joos.cfg)
set(LR1_FILE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joos.lr1)
get_target_property(JLALR_JAR jlalr JAR_FILE)
add_custom_command(
  OUTPUT ${LR1_FILE}
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/cfggen.py
  ${CFG_FILE} |
  ${Java_JAVA_EXECUTABLE} -jar ${JLALR_JAR} > ${LR1_FILE}
  DEPENDS ${CFG_FILE}
  COMMENT "Converting .cfg to .lr1")
add_custom_target(lr1 DEPENDS ${LR1_FILE})

set(TOKENSSPEC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/tokens.lex)
set(TOKENS_FILE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/tokens.lex)
add_custom_command(
  OUTPUT ${TOKENS_FILE}
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/uglyfy.py
  ${TOKENSSPEC_FILE} ${TOKENS_FILE}
  DEPENDS ${TOKENSSPEC_FILE}
  COMMENT "Converting Token Specs to parse tokens")
add_custom_target(tokens DEPENDS ${TOKENS_FILE})

add_library(slc SHARED ${SOURCES} ${HEADER_FILES})
add_dependencies(slc lr1 tokens)
target_include_directories(slc PRIVATE .)

add_executable(joosc Main.cpp)
target_link_libraries(joosc slc)

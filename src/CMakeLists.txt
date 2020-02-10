configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp
)

file(GLOB HEADER_FILES *.hpp)
file(GLOB SOURCES *.cpp)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)

set(CFG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/joos.cfg)
set(LR1_FILE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/joos.lr1)
get_target_property(JLALR_JAR jlalr JAR_FILE)
add_custom_command(
  OUTPUT ${LR1_FILE}
  COMMAND ${Java_JAVA_EXECUTABLE} -jar ${JLALR_JAR}
  < ${CFG_FILE} > ${LR1_FILE}
  DEPENDS ${CFG_FILE}
  COMMENT "Converting .cfg to .lr1"
)
add_custom_target(lr1 DEPENDS ${LR1_FILE})

find_package(PythonInterp)
set(TOKENSPEC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/tokens.lex)
set(TOKEN_FILE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/tokens.lex)
add_custom_command(
  OUTPUT ${TOKEN_FILE}
  COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/uglyfy.py ${TOKENSPEC_FILE} ${TOKEN_FILE}
  DEPENDS ${TOKENSPEC_FILE}
  COMMENT "Converting Token Specs to parse tokens"
)
add_custom_target(tokens DEPENDS ${TOKEN_FILE})

add_library(slc SHARED ${SOURCES} ${HEADER_FILES})
add_dependencies(slc lr1 tokens)
target_include_directories(slc PRIVATE .)

add_executable(joosc Main.cpp)
target_link_libraries(joosc slc)

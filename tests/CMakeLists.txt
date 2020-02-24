configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp
)

file(GLOB SOURCES *.cpp)
add_executable(joosc-test ${SOURCES})
target_include_directories(joosc-test PRIVATE ${slc_SOURCE_DIR}/src)
target_link_libraries(joosc-test slc)

file(COPY data/grammar data/java data/tokens
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata
)

file(MAKE_DIRECTORY
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata/tokens
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata/tokens/a1
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata/tokens/a2
)
file(GLOB_RECURSE JAVA_TESTS data/java/a1/*)

list(REMOVE_ITEM JAVA_TESTS
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_1DigitOctal_1.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_1DigitOctal_2.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_1DigitOctal_3.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_1DigitOctal_4.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_2DigitOctal_1.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_2DigitOctal_2.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_2DigitOctal_3.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_3DigitOctal_1.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_3DigitOctal_2.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_3DigitOctal_3.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_Escapes_NonExistingEscape.java
  ${CMAKE_CURRENT_SOURCE_DIR}/data/java/a1/Je_1_NonJoosConstructs_Unicode.java
)

foreach(java_test ${JAVA_TESTS})
  get_filename_component(JAVA_FILENAME ${java_test} NAME_WE)
  get_filename_component(JAVA_SET_ID ${java_test}/.. ABSOLUTE)
  get_filename_component(JAVA_SET_ID ${JAVA_SET_ID} NAME)
  set(JAVA_TOKENS_FILE
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata/tokens/${JAVA_SET_ID}/${JAVA_FILENAME}.tokens
  )
  add_custom_command(
    OUTPUT ${JAVA_TOKENS_FILE}
    COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/tokenize.py
    ${TOKENS_FILE} ${java_test} > ${JAVA_TOKENS_FILE}
    DEPENDS ${TOKENSSPEC_FILE} ${PROJECT_SOURCE_DIR}/tools/tokenize.py
  )
  list(APPEND JAVA_TOKENS_FILES ${JAVA_TOKENS_FILE})
endforeach(java_test)

add_custom_target(tokens-test DEPENDS ${JAVA_TOKENS_FILES})
add_dependencies(joosc-test tokens-test)

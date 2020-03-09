configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp)

file(GLOB SOURCES *.cpp)
file(GLOB HEADER_FILES *.hpp)
add_executable(joosc-test ${SOURCES} ${HEADER_FILES})
target_include_directories(joosc-test PRIVATE ${slc_SOURCE_DIR}/src)
target_link_libraries(joosc-test slc)

file(COPY data/grammar data/java data/ast data/tokens
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata)

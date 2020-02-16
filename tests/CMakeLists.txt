set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/modules"
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
  DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testdata
)

file(GLOB SOURCES *.cpp)
add_executable(joosc-test ${SOURCES})
target_include_directories(joosc-test PRIVATE ${slc_SOURCE_DIR}/src)
target_link_libraries(joosc-test slc)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(joosc-test)

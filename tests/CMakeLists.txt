configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.hpp
)

file(GLOB SOURCES *.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/Modules"
)
add_executable(joosc-test ${SOURCES})
target_include_directories(joosc-test PRIVATE ${slc_SOURCE_DIR}/src)
target_link_libraries(joosc-test slc)

add_custom_command(TARGET joosc-test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/data
  $<TARGET_FILE_DIR:joosc-test>/testdata
)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(joosc-test)

find_package(Catch2 REQUIRED)

file(GLOB SOURCES *.cpp)

add_executable(joosc-test ${SOURCES})
target_include_directories(joosc-test PRIVATE ${slc_SOURCE_DIR}/src)
target_link_libraries(joosc-test slc Catch2::Catch2)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(joosc-test)

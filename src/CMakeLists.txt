configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
	${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
)

file(GLOB HEADER_FILES *.hpp)
file(GLOB SOURCES *.cpp)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(slc SHARED ${SOURCES} ${HEADER_FILES})
target_include_directories(slc PRIVATE .)

add_custom_command(TARGET slc POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/grammar
	$<TARGET_FILE_DIR:slc>/grammar
)

add_executable(joosc main.cpp)
target_link_libraries(joosc slc)

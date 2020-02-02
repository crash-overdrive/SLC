configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
	${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
)

file(GLOB HEADER_FILES *.hpp)
file(GLOB SOURCES *.cpp)

list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(slc SHARED ${SOURCES} ${HEADER_FILES})
target_include_directories(slc PRIVATE .)

get_target_property(JLALR_JAR jlalr JAR_FILE)
add_custom_command(TARGET slc POST_BUILD
	COMMAND ${Java_JAVA_EXECUTABLE} -jar ${JLALR_JAR}
	< "${CMAKE_CURRENT_SOURCE_DIR}/grammar/joos.cfg"
	> $<TARGET_FILE_DIR:slc>/joos.lr1
	COMMENT "Converting .cfg to .lr1"
)

add_executable(joosc main.cpp)
target_link_libraries(joosc slc)

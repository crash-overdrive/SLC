find_package(Java REQUIRED)
include(UseJava)

add_jar(jlarl
	${CMAKE_CURRENT_SOURCE_DIR}/jlalr/Jlalr1.java
	ENTRY_POINT jlalr/Jlalr1
)

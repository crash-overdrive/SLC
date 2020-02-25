find_package(Java 1.8 REQUIRED)
include(UseJava)

add_jar(jlalr
  ${CMAKE_CURRENT_SOURCE_DIR}/jlalr/Jlalr1.java
  ENTRY_POINT jlalr/Jlalr1
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.joos
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Makefile.joos COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/joos.sh
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/joos)

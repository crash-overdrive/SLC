cmake_minimum_required(VERSION 3.10)

project(slc)

file(GLOB SOURCES src/*.cpp)

add_executable(joosc ${SOURCES})

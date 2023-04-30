cmake_minimum_required(VERSION 3.4.1)

project(MiniDoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

testBuilder_add_include(minidoc include)
testBuilder_add_source(minidoc src/empty.cpp)
testBuilder_build_shared_library(minidoc)

testBuilder_add_source(minidoc_exe src/executable.cpp)
testBuilder_add_library(minidoc_exe minidoc)
testBuilder_build(minidoc_exe EXECUTABLES)

set(MINIDOC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
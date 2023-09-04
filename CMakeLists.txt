cmake_minimum_required(VERSION 3.4.1)

project(MiniDoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

add_subdirectory(darcs-patch)
add_subdirectory(GenericPieceTable)

testBuilder_add_include(minidoc include)
testBuilder_add_source(minidoc src/empty.cpp)
testBuilder_add_library(minidoc darcs_patch)
testBuilder_add_library(minidoc GenericPieceTable)
testBuilder_build_shared_library(minidoc)

testBuilder_add_source(minidoc_exe src/executable.cpp)
testBuilder_add_library(minidoc_exe minidoc)
testBuilder_build(minidoc_exe EXECUTABLES)

if (NOT TARGET gtest AND NOT TARGET gtest_main)
    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(googletest)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif()
endif()


add_subdirectory(tests)

set(MINIDOC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
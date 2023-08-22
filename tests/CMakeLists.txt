cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

testBuilder_add_source(MiniDoc_Tests MiniDoc_Tests.cpp)
testBuilder_add_library(MiniDoc_Tests gtest_main)
testBuilder_add_library(MiniDoc_Tests minidoc)
testBuilder_build(MiniDoc_Tests EXECUTABLES)
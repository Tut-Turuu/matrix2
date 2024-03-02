cmake_minimum_required(VERSION 3.16)

project(matrix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_executable(matrix main.cpp matrix.hpp matrix.cpp)
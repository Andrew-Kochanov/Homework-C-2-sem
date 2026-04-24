cmake_minimum_required(VERSION 3.25)

project(MultiProject C)

enable_testing()

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(src/DFA)

add_subdirectory(tests)
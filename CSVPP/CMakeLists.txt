cmake_minimum_required(VERSION 3.25)
project(CSV)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Создаём библиотеку из файла в поддиректории
add_library(CSV CSVPP.c)

if(MSVC)
  target_compile_options(CSV PRIVATE /W4 /WX /O2 /std:clatest)
else()
  target_compile_options(CSV PRIVATE -Wall -Wextra -Wpedantic -O2 -std=gnu2x)
endif()
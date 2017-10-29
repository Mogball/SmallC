cmake_minimum_required(VERSION 3.8)
project(SmallC)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c small_string.h small_string.c small_json.h small_json.c stack.h stack.c)
add_executable(SmallC ${SOURCE_FILES})
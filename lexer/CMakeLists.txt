cmake_minimum_required(VERSION 3.24)
project(lexer)

set(CMAKE_CXX_STANDARD 17)

file(GLOB libraries "./*/*.cpp")

add_executable(main main.cpp ${libraries})
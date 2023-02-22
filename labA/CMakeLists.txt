cmake_minimum_required(VERSION 3.24)
project(Language)

set(CMAKE_CXX_STANDARD 17)

add_executable(main  expressionTree.cpp  regularExpressions.cpp main.cpp)

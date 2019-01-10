cmake_minimum_required(VERSION 3.10)
project(code)

set(CMAKE_CXX_STANDARD 11)
link_libraries(pthread)

add_executable(code main.cpp)
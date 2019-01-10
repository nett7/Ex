cmake_minimum_required(VERSION 3.10)
project(Pool_threads)

set(CMAKE_CXX_STANDARD 17)

link_libraries(pthread)

add_executable(Pool_threads main.cpp)
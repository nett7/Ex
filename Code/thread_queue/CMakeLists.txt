cmake_minimum_required(VERSION 3.10)
project(thread_queue)

set(CMAKE_CXX_STANDARD 17)

link_libraries(pthread)

add_executable(thread_queue main.cpp)
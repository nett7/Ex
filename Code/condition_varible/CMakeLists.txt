cmake_minimum_required(VERSION 3.10)
project(condition_varible)

set(CMAKE_CXX_STANDARD 17)

link_libraries(pthread)

add_executable(condition_varible main.cpp)
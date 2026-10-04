cmake_minimum_required(VERSION 3.15)

project(SnakeGame CXX)

set(CMAKE_C_STANDARD 99)

find_package(raylib REQUIRED)

add_executable(SnakeGame main.cpp)

target_link_libraries(SnakeGame raylib)

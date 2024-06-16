cmake_minimum_required(VERSION 3.5)
project(parser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(parser main.cpp reader/reader.cpp simba/simba.cpp json/json.cpp)

target_include_directories(parser PRIVATE .)

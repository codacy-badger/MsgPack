cmake_minimum_required(VERSION 3.7)

project(tarigo_msgpack VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
add_subdirectory(test)


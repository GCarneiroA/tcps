
project(rpc)

cmake_minimum_required(VERSION 3.17.1)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
#set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(server server.cpp)
add_executable(client client.cpp)


cmake_minimum_required(VERSION 3.17.1)

project(tcps)

add_executable(tcps server.cpp)
add_executable(tcpc client.cpp)

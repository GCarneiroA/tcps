
cmake_minimum_required(VERSION 3.17.1)

project(tcps)

add_executable(tcps server.cpp)
add_executable(tcpc client.cpp)
add_executable(endians endians.cpp)
add_executable(sizes sizes.cpp)


project(rpc)

<<<<<<< HEAD
cmake_minimum_required(VERSION 3.17.1)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
#set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

=======
#SET(Boost_USE_STATIC_LIBS OFF)
#SET(Boost_USE_MULTITHREAD ON)

#find_package(Boost REQUIRED COMPONENTS system thread regex)
#if (Boost_FOUND)
    #include_directories(${Boost_INCLUDE_DIRS})
    #link_directories(${Boost_LIBRARY_DIRS})
#endif()

#SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

#add_executable(bserver server_bassync.cpp)
#target_link_libraries(bserver ${USED_LIBS})

#add_executable(bclient client_boost.cpp)
#target_link_libraries(bclient ${USED_LIBS})


>>>>>>> 1c903e2... server: update structure
add_executable(server server.cpp)
add_executable(client client.cpp)

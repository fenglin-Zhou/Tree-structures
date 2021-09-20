cmake_minimum_required(VERSION 3.20)
project(tree)
set(CMAKE_CXX_STANDARD 20)

find_package(Threads)
add_executable(tree main.cc binary_search_tree.hpp)
target_link_libraries(tree ${CMAKE_THREAD_LIBS_INIT})

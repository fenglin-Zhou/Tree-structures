cmake_minimum_required(VERSION 3.20)
project(tree)
set(CMAKE_CXX_STANDARD 20)

find_package(Threads)
add_executable(tree main.cc node.h 
btree.h bplustree.h redblacktree.h
binary_search_tree.h avl_tree.h test.h)
target_link_libraries(tree ${CMAKE_THREAD_LIBS_INIT})

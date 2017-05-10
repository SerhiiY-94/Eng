cmake_minimum_required(VERSION 3.2)
project(test_engine)

add_executable(test_engine main.cpp
                           test_common.h
                           test_object_pool.cpp)

target_link_libraries(test_engine engine)
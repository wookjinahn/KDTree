cmake_minimum_required(VERSION 3.23)
project(KDTree)

set(CMAKE_CXX_STANDARD 14)

find_package(camel-euclid-vector REQUIRED)

add_executable(KDTree main.cpp Point.hpp KDTree.hpp)

TARGET_LINK_LIBRARIES(KDTree camel-euclid-vector)
cmake_minimum_required(VERSION 3.23)
project(KDTree_Rosetta)

set(CMAKE_CXX_STANDARD 14)

find_package(camel-euclid-vector REQUIRED)

add_executable(KDTree_Rosetta main.cpp Point.hpp KDTree.hpp)

TARGET_LINK_LIBRARIES( KDTree_Rosetta camel-euclid-vector )
cmake_minimum_required(VERSION 3.26)
project(step_counting)

set(CMAKE_CXX_STANDARD 11)

add_executable(step_counting main.cpp
        App.h
        App.cpp)

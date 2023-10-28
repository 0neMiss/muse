cmake_minimum_required(VERSION 3.20)

project(muse)

set(CMAKE_CXX_STANDARD 20)

add_executable(muse src/main.cpp
        src/tokenization.hpp
        src/parser.hpp)
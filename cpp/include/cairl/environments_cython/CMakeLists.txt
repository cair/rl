cmake_minimum_required(VERSION 3.19)
project(classic_control)


set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

find_package(Python3 COMPONENTS Interpreter Development Development.Embed REQUIRED)
find_package(pybind11 CONFIG REQUIRED)



add_executable(wumpus_test
        main.cpp
        cppWumpus.cpp

        )
target_include_directories(wumpus_test PUBLIC ./)
target_link_libraries(
        wumpus_test
        PUBLIC
        Python3::Module
        Python3::Python
        pybind11::pybind11
)
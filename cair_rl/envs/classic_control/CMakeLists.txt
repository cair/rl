cmake_minimum_required(VERSION 3.19)
project(classic_control)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)


find_package(pybind11 CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(Python3 COMPONENTS Interpreter NumPy REQUIRED)

if (LIBRARY_TYPE STREQUAL "SHARED")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()


if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    if (WIN32)
        add_compile_options(/Ox /utf-8)
    else ()
        add_compile_options(-Ofast -march=native -ffast-math -Wall -fstack-protector-all)
    endif ()
endif ()

if (IS_PYBIND_BUILD)
    set(LIBRARY_TYPE SHARED)
elseif (WIN32)
    set(LIBRARY_TYPE STATIC)
endif ()



set(classic_control_cartpole_files
        include/envs/classic_control/cartpole/cartpole.h
        src/envs/classic_control/cartpole/cartpole.cpp
        include/spaces/Discrete.h include/spaces/Space.h include/rendering/renderer.h include/envs/Env.h)

add_subdirectory(extern/random)
add_subdirectory(extern/spdlog)
set_target_properties( spdlog
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

add_library(classic_control_cartpole SHARED ${classic_control_cartpole_files})
target_include_directories(classic_control_cartpole PUBLIC
        "include"
)

target_link_libraries(classic_control_cartpole PUBLIC
        effolkronium_random
        spdlog::spdlog
        opencv_core
        opencv_highgui
        Python3::NumPy
)


if (NOT IS_PYBIND_BUILD)
    add_executable(classic_control_exec main.cpp)
    target_link_libraries(classic_control_exec PRIVATE classic_control_cartpole)
endif ()
#############################################################################################
#####
##### PYBIND11 Builds
#####
#############################################################################################
file(GLOB BIND_FILES ${PROJECT_SOURCE_DIR}/bindings/*.cpp)
file(GLOB BIND_FILES_CONVERTERS  ${PROJECT_SOURCE_DIR}/bindings/converters/*.cpp)
set(ALL_BIND_FILES ${BIND_FILES_CONVERTERS} ${BIND_FILES})

pybind11_add_module(gym ${ALL_BIND_FILES} ${BIND_FILES_CONVERTERS})
target_link_libraries(gym PUBLIC classic_control_cartpole)
MESSAGE("Note: To build python bindings, run 'setup.py clean install'.")

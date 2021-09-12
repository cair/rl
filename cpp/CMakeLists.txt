cmake_minimum_required(VERSION 3.19)
project(classic_control)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Lock this section so that parallel CMake runs won't clash on checkout in the same directory
#file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)


#############################################################################################
#####
##### CMAKE Includes
#####
#############################################################################################
include(cmake/GitUpdate.cmake)
include(cmake/pmm.cmake)
include(cmake/CPM.cmake)
include(cmake/AddSubdirectoryOptions.cmake)



#############################################################################################
#####
##### C++ Compiler Setup
#####
#############################################################################################
set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)


if (LIBRARY_TYPE STREQUAL "SHARED")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

if (UNIX)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif ()


if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    if (WIN32)
        add_compile_options(/Ox /utf-8)
    else ()
        add_compile_options(-Ofast -march=native -ffast-math -Wall)
    endif ()
endif ()

if (IS_PYBIND_BUILD)
    set(LIBRARY_TYPE SHARED)
elseif (WIN32)
    set(LIBRARY_TYPE STATIC)
endif ()


#############################################################################################
#####
##### VCPKG - Configuration
#####
#############################################################################################
if (WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "" FORCE)
    set(VCPKG_STATIC_LINKING ON)
elseif (UNIX)
    set(VCPKG_TARGET_TRIPLET x64-linux)
endif ()

#############################################################################################
#####
##### VCPKG - Dependencies
#####
#############################################################################################
set(CAIRL_GYM_DEPENDENCIES
        spdlog
        effolkronium-random
        python3
        sdl2-videodriver[offscreen]
        xtensor[xsimd])



set(CAIRL_GYM_LINK_TARGETS
        spdlog::spdlog
        effolkronium_random
        opencv_core
        opencv_highgui
        Python::NumPy
        xtensor::optimize
        xtensor::use_xsimd
)


# VCPKG - Install Depedencies
pmm(
        VERBOSE
        DEBUG
        VCPKG
        REVISION master
        REQUIRES ${CAIRL_GYM_DEPENDENCIES}
        TRIPLET ${VCPKG_TARGET_TRIPLET}
        PORTS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-ports/sdl2-videodriver
)




#############################################################################################
#####
##### CMAKE - Find Packages
#####
#############################################################################################
find_package(OpenCV CONFIG REQUIRED )
find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(Python COMPONENTS Interpreter NumPy REQUIRED)



find_package(effolkronium_random CONFIG REQUIRED)
find_package(xtensor CONFIG REQUIRED)


find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#############################################################################################
#####
##### CPM - Add Dependencies that are not in vcpkg
#####
#############################################################################################

CPMAddPackage(
        NAME xtensor_python_cair
        GIT_REPOSITORY "https://github.com/xtensor-stack/xtensor-python.git"
        GIT_TAG 0.25.3
)


#############################################################################################
#####
##### CMAKE - Third-party Dependencies
##### #
#############################################################################################
set(CAIRL_THIRD_PARTY_SOURCES "")
set(CAIRL_THIRD_PARTY_INCLUDE_DIRS "")
set(CAIRL_THIRD_PARTY_LINKER "")

include(third_party.cmake)

#############################################################################################
#####
##### CMAKE - Source Files
#####
#############################################################################################
set(CAIRL_GYM_SOURCE_FILES
        # CartPole
        include/cairl/environments/classic_control/cartpole.h
        include/cairl/environments/classic_control/mountain_car.h
        include/cairl/environments/classic_control/mountain_car_continuous.h


        # Utils
        include/cairl/utils/Timer.h
        include/cairl/utils/File.h
        include/cairl/utils/VectorWrapper.h
        include/cairl/utils/xtensor_cv_convertions.h

        # Spaces
        include/cairl/spaces/Space.h
        include/cairl/spaces/Discrete.h
        include/cairl/spaces/Box.h

        # Renderer
        include/cairl/rendering/renderer.h

        # Wrappers
        include/cairl/wrappers/TimeLimit.h

        # Env
        include/cairl/environments/Env.h

        include/cairl/defs.h


        include/cairl/envs.h



        include/cairl/wrappers/Flatten.h
        include/cairl/environments/classic_control/acrobot.h
        include/cairl/environments/classic_control/pendulum.h include/cairl/contrib/environments/flashrl/envs.h)



#############################################################################################
#####
##### CMAKE - Add Library Targets
#####
#############################################################################################
add_library(
        CAIRL_GYM_LIB
        ${LIBRARY_TYPE}
        ${CAIRL_GYM_SOURCE_FILES}
        ${CAIRL_THIRD_PARTY_SOURCES}
)

target_include_directories(
        CAIRL_GYM_LIB
        PUBLIC
        include
        ${CAIRL_THIRD_PARTY_INCLUDE_DIRS}
        ${VCPKG_INSTALLED_DIR}/x64-linux/include
)

target_link_libraries(
        CAIRL_GYM_LIB
        PUBLIC
        ${CAIRL_THIRD_PARTY_LINKER}
        ${CAIRL_GYM_LINK_TARGETS}
)

#############################################################################################
#####
##### CMAKE - Add Executable
#####
#############################################################################################
if (NOT IS_PYBIND_BUILD)
    add_executable(CAIRL_Exec main.cpp)
    target_link_libraries(
            CAIRL_Exec
            PRIVATE
            CAIRL_GYM_LIB
    )
endif()

#############################################################################################
#####
##### PyBind11 - Build Target
#####
#############################################################################################
file(GLOB BIND_FILES
        ${PROJECT_SOURCE_DIR}/src/pybind/*.cpp
        ${PROJECT_SOURCE_DIR}/src/pybind/converters/*.cpp
)

pybind11_add_module(gym
        ${BIND_FILES}
)
target_link_libraries(
        gym
        PUBLIC
        CAIRL_GYM_LIB
)
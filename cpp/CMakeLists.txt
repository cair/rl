cmake_minimum_required(VERSION 3.19)
project(classic_control)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Lock this section so that parallel CMake runs won't clash on checkout in the same directory
file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)


#############################################################################################
#####
##### CMAKE Includes
#####
#############################################################################################
include(cmake/pmm.cmake)
include(cmake/CPM.cmake)
include(cmake/AddSubdirectoryOptions.cmake)

#############################################################################################
#####
##### Dependencies
#####
#############################################################################################

add_subdirectory(extern/flashrl-lightspark)

CPMADDPackage(
        NAME Armadillo
        GIT_REPOSITORY "https://gitlab.com/conradsnicta/armadillo-code.git"
        GIT_TAG 10.6.x
)

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
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "" FORCE)  # -static ?
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
        #opencv
        python3
        sdl2-videodriver[offscreen]
)



set(CAIRL_GYM_LINK_TARGETS
        spdlog::spdlog
        effolkronium_random
        opencv_core
        opencv_highgui
        Python3::NumPy
        lighspark_lib  # extern/flashrl-lightspark
        carma
        armadillo
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
#find_package(sdl2-videodriver CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED )
find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

find_package(Python3 COMPONENTS Interpreter NumPy REQUIRED)
find_package(effolkronium_random CONFIG REQUIRED)
find_package(Armadillo REQUIRED)


find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


#############################################################################################
#####
##### CMAKE - Source Files
#####
#############################################################################################
set(CAIRL_GYM_SOURCE_FILES
        # CartPole
        include/cairl/environments/classic_control/cartpole.h

        # Mountain Car
        include/cairl/environments/classic_control/mountain_car.h
        include/cairl/environments/classic_control/mountain_car_continuous.h

        # Utils
        include/cairl/utils/Timer.h

        # Spaces
        include/cairl/spaces/Space.h
        include/cairl/spaces/Discrete.h
        include/cairl/spaces/Box.h

        # Renderer
        include/cairl/rendering/renderer.h

        # Env
        include/cairl/environments/Env.h
        include/cairl/environments/flashrl/FlashEnv.h
        include/cairl/defs.h


        include/cairl/environments/flashrl/lightspark/LightsparkRunner.h
        include/cairl/environments/flashrl/FlashEngineRunner.h
        include/cairl/utils/File.h
        include/cairl/environments/flashrl/lightspark/CaiRLEngineData.h

        include/cairl/utils/arma_cv_convertions.h

        include/cairl/environments/flashrl/games/Multitask.h
        include/cairl/envs.h
        include/cairl/utils/VectorWrapper.h)


#############################################################################################
#####
##### CMAKE - Add Library Targets
#####
#############################################################################################
add_library(CAIRL_GYM_LIB ${LIBRARY_TYPE} ${CAIRL_GYM_SOURCE_FILES})

target_include_directories(CAIRL_GYM_LIB
        PUBLIC
        include
        ${VCPKG_INSTALLED_DIR}/x64-linux/include
)
target_link_libraries(CAIRL_GYM_LIB
        PUBLIC
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
#if(IS_PYBIND_BUILD)

add_library(carma INTERFACE)
target_include_directories(carma INTERFACE extern/carma/include)
target_link_libraries(carma INTERFACE Python3::NumPy Python3::Module pybind11::pybind11)



file(GLOB BIND_FILES ${PROJECT_SOURCE_DIR}/src/pybind/*.cpp)
file(GLOB BIND_FILES_CONVERTERS  ${PROJECT_SOURCE_DIR}/src/pybind/converters/*.cpp)
set(ALL_BIND_FILES ${BIND_FILES_CONVERTERS} ${BIND_FILES})

pybind11_add_module(gym ${ALL_BIND_FILES} ${BIND_FILES_CONVERTERS})
message( ${PROJECT_SOURCE_DIR}/cpp/src/pybind)
target_link_libraries(gym PUBLIC CAIRL_GYM_LIB carma)
MESSAGE("Note: To build python bindings, run 'setup.py clean install'.")
#endif()


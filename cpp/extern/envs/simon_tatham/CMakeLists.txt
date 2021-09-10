cmake_minimum_required(VERSION 3.20)
project(cairl_simon_tatham_puzzle)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(xpm REQUIRED IMPORTED_TARGET xpm)

find_package(Boost REQUIRED COMPONENTS thread)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


#include_directories(${GTK_INCLUDE_DIRS})
#link_directories(${GTK_LIBRARY_DIRS})
#add_definitions(${GTK_CFLAGS_OTHER})




include(FetchContent)
FetchContent_Declare(
        simon_tatham_puzzles
        URL https://www.chiark.greenend.org.uk/~sgtatham/puzzles/puzzles.tar.gz
        URL_MD5 c3b9eb5deab40e970e98978ea86e80db

)
FetchContent_MakeAvailable(simon_tatham_puzzles)

set(puzzle_path ${PROJECT_BINARY_DIR}/_deps/simon_tatham_puzzles-src/)

list(TRANSFORM platform_common_sources PREPEND ${puzzle_path})

add_library(simon_tatham_common STATIC
        ${puzzle_path}combi.c
        ${puzzle_path}divvy.c

        ${puzzle_path}dsf.c
        ${puzzle_path}findloop.c
        ${puzzle_path}grid.c
        ${puzzle_path}latin.c
        ${puzzle_path}laydomino.c
        ${puzzle_path}loopgen.c
        ${puzzle_path}malloc.c
        ${puzzle_path}matching.c
        ${puzzle_path}midend.c
        ${puzzle_path}misc.c
        ${puzzle_path}penrose.c
        ${puzzle_path}ps.c
        ${puzzle_path}random.c
        ${puzzle_path}sort.c
        ${puzzle_path}tdq.c
        ${puzzle_path}tree234.c
        ${puzzle_path}version.c
        ${puzzle_path}no-icon.c
        #${puzzle_path}gtk.c
        gtk.c
        # Platform stuff
        custom_impl.c
        ${puzzle_path}drawing.c  # either drawing
        #${puzzle_path}nullfe.c  # or no drawing

        ${puzzle_path}printing.c
       #${platform_common_sources}
)
target_compile_options(simon_tatham_common PUBLIC -fpermissive)
#target_compile_definitions(simon_tatham_common PUBLIC COMBINED)
target_include_directories(simon_tatham_common
        PUBLIC
        ${puzzle_path}
        ${GTK_INCLUDE_DIRS}
        ./

)
target_link_libraries(simon_tatham_common PUBLIC
        ${GTK_LIBRARIES}
        Boost::thread
        PkgConfig::glib
)
#target_compile_definitions(simon_tatham_common PUBLIC DEBUGGING)

message( ${puzzle_path})


add_library(cairl_simon_tatham_puzzle SHARED /dev/null)
set_target_properties(cairl_simon_tatham_puzzle PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(cairl_simon_tatham_puzzle
        PUBLIC
        simon_tatham_common
    )
target_include_directories(cairl_simon_tatham_puzzle
        INTERFACE
        include
        ${puzzle_path_INCLUDE_DIRS}
        )



add_executable(cairl_simon_tatham_puzzle_bin main.cpp)

target_link_libraries(cairl_simon_tatham_puzzle_bin cairl_simon_tatham_puzzle)
#  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
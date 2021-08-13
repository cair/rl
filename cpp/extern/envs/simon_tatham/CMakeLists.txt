cmake_minimum_required(VERSION 3.20)
project(cairl_simon_tatham_puzzle)

include(ExternalProject)

ExternalProject_Add(simon_tatham_puzzles
        URL https://www.chiark.greenend.org.uk/~sgtatham/puzzles/puzzles.tar.gz
        URL_MD5 c3b9eb5deab40e970e98978ea86e80db
        PREFIX ${CMAKE_BINARY_DIR}/simon_tatham_puzzles
        INSTALL_DIR ${INSTALL_DIR}
        CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${TOOLCHAIN_FILE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DCMAKE_BUILD_TYPE:STRING=Release
        )
#  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
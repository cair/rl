cmake_minimum_required(VERSION 3.19)
project(lighspark_lib)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#############################################################################################
#####
##### C++ Compiler Setup
#####
#############################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)


#find_package(IPE REQUIRED)
find_package(Cairo REQUIRED)
find_package(GLIB REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

include(cmake/AddSubdirectoryOptions.cmake)

add_subdirectory_options(
        PATH lightspark
        OPTIONS
        "COMPILE_LIGHTSPARK TRUE"
        "COMPILE_TIGHTSPARK TRUE"
        "ENABLE_CURL TRUE"
        "ENABLE_SSE2 FALSE"  # This would be nice
        "COMPILE_NPAPI_PLUGIN FALSE"
        "COMPILE_PPAPI_PLUGIN FALSE"
        "ENABLE_GLES2 FALSE"
        "ENABLE_LIBAVCODEC FALSE"  # TODO - might need?
        "ENABLE_RTMP FALSE" # TODO - might need?
        "ENABLE_LLVM FALSE"
        "ENABLE_PROFILING FALSE"
        "ENABLE_MEMORY_USAGE_PROFILING FALSE"
)
target_compile_definitions(spark
        PUBLIC
        LIGHTSPARK_64=1   # 64 bits
        HAVE_ATOMIC=1
)
target_compile_options(spark PRIVATE
        -Wall
        -pipe
        -D__STDC_CONSTANT_MACROS
        $<$<COMPILE_LANGUAGE:CXX>:-fvisibility=default>  #
)

target_include_directories(spark PUBLIC
        ${GLIB_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        lightspark/src
        lightspark/src/scripting
        ${GLIB_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lightspark/src>
        #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lightspark/src/scripting>
        #$<INSTALL_INTERFACE:lightspark/src>
        #$<INSTALL_INTERFACE:lightspark/src/scripting>
        )


add_executable(lightspark_lib_exec main.cpp src/swf.h)
target_link_libraries(lightspark_lib_exec
        PUBLIC
        spark
)

add_library(lighspark_lib INTERFACE)
target_include_directories(
        lighspark_lib
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
        lighspark_lib
        INTERFACE
        spark
)


install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/modules/
        DESTINATION modules
)
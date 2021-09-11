cmake_minimum_required(VERSION 3.19)
project(cairl_deep_rts)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/CPM.cmake)

#############################################################################################
#####
##### C++ Compiler Setup
#####
#############################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

CPMAddPackage(
        NAME deep-rts
        GITHUB_REPOSITORY cair/deep-rts
        GIT_TAG main
        OPTIONS
        "ENABLE_LOCK OFF"
        "BUILD_DOCS OFF"
)
#add_library(cairl-deep-rts INTERFACE)
#target_link_libraries(cairl-deep-rts PUBLIC DeepRTSLib)
#target_include_directories(cairl-deep-rts INTERFACE )
#
#

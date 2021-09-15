cmake_minimum_required(VERSION 3.20)
project(cairl_microrts)

include(../../../cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)


find_package(JNI REQUIRED)
find_package(Java REQUIRED COMPONENTS Development)
include(UseJava)

set(CAIRL_JAVA_VERSION ${Java_VERSION_MAJOR}.${Java_VERSION_MINOR})




CPMAddPackage(
        NAME showdownai
        GITHUB_REPOSITORY scotchkorean27/showdownai2.0
        GIT_TAG master
)
set(SHOWDOWNAI_SRC_DIR ${microrts_SOURCE_DIR}/src)
#set(MICRORTS_LIB_DIR ${microrts_SOURCE_DIR}/lib)
# Compile MicroRTS source files
set(CMAKE_JAVA_COMPILE_FLAGS "-source" ${CAIRL_JAVA_VERSION} "-target" ${CAIRL_JAVA_VERSION})
set(CMAKE_JAVA_INCLUDE_PATH
        ${SHOWDOWNAI_SRC_DIR}
        )



add_jar(ShowdownAI
        SOURCES
        ${SHOWDOWNAI_SRC_DIR}/pokemonai/game/Runner.java

        INCLUDE_JARS


        OUTPUT_NAME
        MicroRTS

        VERSION 1.0.0
        GENERATE_NATIVE_HEADERS ShowdownAI-native

        )
get_target_property(_jarFile ShowdownAI JAR_FILE)

## Create directory for combine ops
#add_custom_command(
#        TARGET MicroRTS POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/ShowdownAI
#)
#
#add_custom_command(
#        TARGET MicroRTS POST_BUILD
#        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/combine_jar.sh ${MICRORTS_LIB_DIR} ${_jarFile}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/MicroRTS
#)

# | xargs -n 1 jar xvf


add_library(cairl_showdownai_lib STATIC /dev/null)
target_include_directories(
        cairl_showdownai_lib PUBLIC
        include
        ${JAVA_INCLUDE_PATH}
        ${JAVA_INCLUDE_PATH2}
        ${JAVA_AWT_INCLUDE_PATH}
)
target_link_libraries(cairl_showdownai_lib PUBLIC
        ${JAVA_JVM_LIBRARY}
        ${JAVA_AWT_LIBRARY}
        INTERFACE
        ShowdownAI-native
        )
set_target_properties(cairl_showdownai_lib PROPERTIES LINKER_LANGUAGE CXX)

# https://github.com/scotchkorean27/showdownai2.0/blob/master/src/pokemonai/game/Game.java'
# still todo- might even just port everything to c++


add_executable(cairl_showdownai_exec main.cpp)
target_link_libraries(cairl_showdownai_exec
        PUBLIC cairl_showdownai_lib
        INTERFACE
        ShowdownAI-native
        )


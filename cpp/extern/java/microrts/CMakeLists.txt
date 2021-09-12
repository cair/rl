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
        NAME microrts
        GITHUB_REPOSITORY santiontanon/microrts
        GIT_TAG db59145a0872f500f7ca4d128109cb74455d0be0
)
set(MICRORTS_SRC_DIR ${microrts_SOURCE_DIR}/src)
set(MICRORTS_LIB_DIR ${microrts_SOURCE_DIR}/lib)
# Compile MicroRTS source files
set(CMAKE_JAVA_COMPILE_FLAGS "-source" ${CAIRL_JAVA_VERSION} "-target" ${CAIRL_JAVA_VERSION})
set(CMAKE_JAVA_INCLUDE_PATH
        ${MICRORTS_LIB_DIR}
        ${MICRORTS_SRC_DIR}
        )


message(${MICRORTS_SRC_DIR}/rts/MicroRTS.java)
add_jar(MicroRTS
        SOURCES
        ${MICRORTS_SRC_DIR}/rts/MicroRTS.java

        INCLUDE_JARS
        ${MICRORTS_LIB_DIR}/jdom.jar
        ${MICRORTS_LIB_DIR}/minimal-json-0.9.4.jar
        ${MICRORTS_LIB_DIR}/weka.jar

        OUTPUT_NAME
        MicroRTS

        VERSION 1.0.0
        GENERATE_NATIVE_HEADERS MicroRTS-native

)
get_target_property(_jarFile MicroRTS JAR_FILE)

# Create directory for combine ops
add_custom_command(
        TARGET MicroRTS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/MicroRTS
)

add_custom_command(
        TARGET MicroRTS POST_BUILD
        COMMAND bash combine_jar.sh ${MICRORTS_LIB_DIR} ${_jarFile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/MicroRTS
        )

# | xargs -n 1 jar xvf


add_library(cairl_microrts_lib STATIC /dev/null)
target_include_directories(
        cairl_microrts_lib PUBLIC
        include
        ${JAVA_INCLUDE_PATH}
        ${JAVA_INCLUDE_PATH2}
        ${JAVA_AWT_INCLUDE_PATH}
)
target_link_libraries(cairl_microrts_lib PUBLIC
        ${JAVA_JVM_LIBRARY}
        ${JAVA_AWT_LIBRARY}
        INTERFACE
        MicroRTS-native
)
set_target_properties(cairl_microrts_lib PROPERTIES LINKER_LANGUAGE CXX)



add_executable(cairl_microrts_exec main.cpp)
target_link_libraries(cairl_microrts_exec
        PUBLIC cairl_microrts_lib
        INTERFACE         MicroRTS-native
        )


cmake_minimum_required(VERSION 3.19)
project(classic_control)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD /std:c++17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if (LIBRARY_TYPE STREQUAL "SHARED")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()


add_subdirectory(../../extern/random RANDOM)
add_subdirectory(../../extern/spdlog SPD)
set_target_properties(spdlog
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        )


set(cairl_spaces_files
        ../../include/cairl/spaces/Space.h
        ../../include/cairl/spaces/Box.h
        ../../include/cairl/spaces/Discrete.h
)


if (NOT IS_PYBIND_BUILD)
    add_executable(cairl_spaces_exec test_spaces.cpp)
    target_link_libraries(cairl_spaces_exec PRIVATE         spdlog::spdlog
            effolkronium_random)
    target_include_directories(cairl_spaces_exec PRIVATE
            ../../include/cairl/spaces
            ../../include
            )

endif ()

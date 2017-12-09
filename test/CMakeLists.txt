cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Gui-Test" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

if(UNIX)
    add_definitions(-DX11 -std=c++11)
endif(UNIX)

add_subdirectory(./../gui gui)

add_definitions(${gui_defines})
message(STATUS "app gui_defines: ${gui_defines}")

# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR}
                    ${gui_include}
)

if (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
    set(APPEND CMAKE_CXX_FLAGS /bigobj)
endif()

enable_testing()

set(tests
    WindowTest
    BorderTest
    hello_world
)

message(STATUS "gui_libraries: ${gui_libraries}")

foreach(test ${tests})
    if (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
        add_compile_options(/Y-)
    endif()
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${gui_libraries})
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
endforeach(test)

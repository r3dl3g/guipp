cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui-lib" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if(UNIX)
  add_definitions(-DX11 -std=c++11)
endif(UNIX)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_definitions(-Wno-switch)
endif()

add_subdirectory(gui)
add_subdirectory(test)

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

message(STATUS "gui_Include:" ${gui_Include})

# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR}
                    ${gui_Include}
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

enable_testing()

set(tests
  WindowTest
  BorderTest
  hello_world
)

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} gui++)
    if(UNIX)
      #SET_TARGET_PROPERTIES(${test} PROPERTIES PREFIX "lib")
      target_link_libraries(${test} stdc++fs X11 Xft fontconfig pthread)
    endif(UNIX)
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
endforeach(test)

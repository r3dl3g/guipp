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

# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR}
                    ${gui_Include}
)

link_directories(${gui_core_Lib}
                 ${gui_draw_Lib}
                 ${gui_win_Lib}
                 ${gui_io_Lib}
                 ${gui_ctrl_Lib}
#                 ${ibr_std_lib_Lib}
)

#link_directories("../boost/stage/lib")

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
    target_link_libraries(${test} gui_core gui_draw gui_win gui_ctrl gui_io)
    if(UNIX)
      #SET_TARGET_PROPERTIES(${test} PROPERTIES PREFIX "lib")
      target_link_libraries(${test} stdc++fs X11 Xft fontconfig pthread Xext)
    endif(UNIX)
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
endforeach(test)

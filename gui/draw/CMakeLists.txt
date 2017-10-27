cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui_draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../core gui_core)

if(NOT TARGET gui_draw)

include_directories(${gui_Include})

file(GLOB SOURCE_FILES "*.h" "*.cpp")

link_directories(${gui_draw_Lib})

add_library(gui_draw ${SOURCE_FILES})

endif()

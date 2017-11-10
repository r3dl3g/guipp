cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui_io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../draw gui_draw)

if(NOT TARGET gui_io)

include_directories(${gui_Include})

file(GLOB SOURCE_FILES "*.h" "*.cpp")

link_directories(${gui_io_Lib})

add_library(gui_io ${SOURCE_FILES})

endif()

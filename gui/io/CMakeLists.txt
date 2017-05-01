cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui_io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../draw gui_draw)

set (gui_io_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     ${gui_draw_Include}
)

set (gui_io_Include ${gui_io_Include} PARENT_SCOPE)

if(NOT TARGET gui_io)

include_directories(${gui_io_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${gui_io_Lib})

add_library(gui_io ${SOURCE_FILES})

endif()

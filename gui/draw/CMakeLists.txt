cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui_draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../core gui_core)

set (gui_draw_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     ${gui_core_Include}
)

set (gui_draw_Include ${gui_draw_Include} PARENT_SCOPE)

if(NOT TARGET gui_draw)

include_directories(${gui_draw_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${gui_draw_Lib})

add_library(gui_draw ${SOURCE_FILES})

endif()

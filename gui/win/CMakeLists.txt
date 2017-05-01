cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui_win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../core gui_core)

set (gui_win_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     ${gui_core_Include}
)
set (gui_win_Include ${gui_win_Include} PARENT_SCOPE)

if(NOT TARGET gui_win)

include_directories(${gui_win_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${gui_win_Lib})

add_library(gui_win ${SOURCE_FILES})

target_link_libraries(gui_win gui_core)

endif()


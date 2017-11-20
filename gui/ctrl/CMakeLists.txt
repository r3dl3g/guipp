cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui_ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../win gui_win)
add_subdirectory(./../io gui_io)

if(NOT TARGET gui_ctrl)

include_directories(${gui_Include})

file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")

link_directories(${gui_ctrl_Lib})

add_library(gui_ctrl ${SOURCE_FILES})

target_link_libraries(gui_ctrl gui_core gui_draw gui_win gui_io)

endif()

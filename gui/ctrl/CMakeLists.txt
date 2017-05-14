cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui_ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../win gui_win)
add_subdirectory(./../io gui_io)

set (gui_ctrl_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     ${gui_win_Include}
     ${gui_io_Include}
)
set (gui_ctrl_Include ${gui_ctrl_Include} PARENT_SCOPE)

if(NOT TARGET gui_ctrl)

include_directories(${gui_ctrl_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${gui_ctrl_Lib})

add_library(gui_ctrl ${SOURCE_FILES})

target_link_libraries(gui_ctrl gui_win gui_draw gui_io)

endif()

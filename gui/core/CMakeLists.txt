cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui_core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library
add_subdirectory(./../../ibr_std_lib ibr_std_lib)

set (gui_core_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     ${ibr_std_lib_Include}
     /usr/include/freetype2
)
set (gui_core_Include ${gui_core_Include} PARENT_SCOPE)

if(NOT TARGET gui_core)

include_directories(${gui_core_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${gui_core_Lib})

add_library(gui_core ${SOURCE_FILES})

endif()

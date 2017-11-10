cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui_core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library
# add_subdirectory(./../../ibr_std_lib ibr_std_lib)

if(NOT TARGET gui_core)

include_directories(${gui_Include})

file(GLOB SOURCE_FILES "*.h" "*.cpp")

link_directories(${gui_core_Lib})

add_library(gui_core ${SOURCE_FILES})

endif()

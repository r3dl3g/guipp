cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../core core)

set (draw_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     "../../boost"
     ${core_Include}
)

set (draw_Include ${draw_Include} PARENT_SCOPE)

if(NOT TARGET draw)

include_directories(${draw_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${draw_Lib})

add_library(draw ${SOURCE_FILES})

endif()

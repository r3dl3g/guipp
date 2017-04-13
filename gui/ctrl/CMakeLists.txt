cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../win win)
add_subdirectory(./../draw draw)

set (ctrl_Include
     ${CMAKE_CURRENT_SOURCE_DIR}/sources
     ${draw_Include}
     ${win_Include}
)
set (ctrl_Include ${ctrl_Include} PARENT_SCOPE)

if(NOT TARGET ctrl)

include_directories(${ctrl_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${ctrl_Lib})

add_library(ctrl ${SOURCE_FILES})

target_link_libraries(ctrl win draw core)

endif()


cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

add_subdirectory(./../draw draw)

set (win_Include
	${CMAKE_CURRENT_SOURCE_DIR}/sources
	${draw_Include}
)
set (win_Include ${win_Include} PARENT_SCOPE)

if(NOT TARGET win)

include_directories(${win_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${win_Lib})

add_library(win ${SOURCE_FILES})

endif()

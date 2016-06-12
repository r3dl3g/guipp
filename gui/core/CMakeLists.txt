cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

set (core_Include
	${CMAKE_CURRENT_SOURCE_DIR}/sources
)
set (core_Include ${core_Include} PARENT_SCOPE)

if(NOT TARGET core)

include_directories(${core_Include})

file(GLOB SOURCE_FILES "sources/*.*")

link_directories(${core_Lib})

add_library(core ${SOURCE_FILES})

endif()

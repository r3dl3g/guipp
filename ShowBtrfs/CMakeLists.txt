cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("ShowBtrfs" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

if (UNIX)
  add_definitions(-DX11 -std=c++11)
endif()

add_subdirectory(./../gui gui)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)


# add the binary tree to the search path for include files
# so that we will find Version.h

include_directories(${PROJECT_BINARY_DIR} ${gui_include} ../boost)
add_definitions(${gui_defines})

#if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#    add_definitions(/Y- /bigobj)
#endif()

add_executable(ShowBtrfs WIN32 ShowBtrfs.cpp)
target_link_libraries(ShowBtrfs ${gui_libraries} ${gui_sys_libraries})

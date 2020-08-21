cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("TimeTracker" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

add_subdirectory(./../gui gui)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)


# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR} ${gui_include} ../boost)
add_definitions(${gui_defines})
add_definitions(-std=c++14)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(/Y- /bigobj)
endif()

add_executable(TimeTracker WIN32 TimeTracker.cpp)
target_link_libraries(TimeTracker ${gui_app_libraries} ${gui_sys_libraries})
set_target_properties(TimeTracker PROPERTIES FOLDER apps)

install(TARGETS TimeTracker DESTINATION bin)

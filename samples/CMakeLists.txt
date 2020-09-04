cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Samples" CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if (APPLE)
  add_definitions(-DCOCOA -std=c++11)
elseif (UNIX)
  add_definitions(-DX11 -std=c++11)
endif()

add_subdirectory(./../gui gui)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

add_definitions(${gui_defines})
message(STATUS "app gui_defines: ${gui_defines}")
message(STATUS "app gui_include: ${gui_include}")
message(STATUS "app gui_libraries: ${gui_libraries}")
message(STATUS "app gui_sys_libraries: ${gui_sys_libraries}")


# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR} ${gui_include} ../boost)

set(apps
    WindowTest
    BorderTest
    hello_world
    filemanager
    list_app
    tree_app
    new_border_layout
    attach_all_layout
    dialogs
    split_view
    diagrams
)

message(STATUS "gui_libraries: ${gui_libraries}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(/Y- /bigobj)
endif()

foreach(test ${apps})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${gui_app_libraries} ${gui_sys_libraries})
    install(TARGETS ${test} DESTINATION bin)
    set_target_properties(${test} PROPERTIES FOLDER apps)
endforeach(test)

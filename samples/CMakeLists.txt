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

add_definitions(${GUIPP_CXX_FLAGS})
message(STATUS "app GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
message(STATUS "app GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
message(STATUS "app GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")
message(STATUS "app GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")


# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS} ../boost)

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
    covid19
)

message(STATUS "GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(/Y- /bigobj)
endif()

foreach(test ${apps})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    install(TARGETS ${test} DESTINATION bin)
    set_target_properties(${test} PROPERTIES FOLDER apps CXX_STANDARD 11)
endforeach(test)

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Gui-Test" CXX)

set(BUILD_FOR_ARM OFF CACHE BOOL "On to build for arm architecture, Off for host architecture. default Off")

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

enable_testing()

add_subdirectory(./../testlib testlib)
add_subdirectory(./../testing testing)

set(tests
    datamap_test
    diagram_test
    pixmap_test
    drawer_test
    icon_test
    stretch_test
    frames_test
    csv_test
)

message(STATUS "GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(/Y- /bigobj)
endif()

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} testing testlib ${GUIPP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
    set_target_properties(${test} PROPERTIES FOLDER tests)
endforeach(test)

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Gui-Test" CXX)

set(BUILD_FOR_ARM OFF CACHE BOOL "On to build for arm architecture, Off for host architecture. default Off")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

add_subdirectory(./../gui gui)

if (GUIPP_USE_QT)
  message(STATUS "GUIPP test: build with Qt5!")
  find_package(Qt5Widgets REQUIRED)
endif()

add_definitions(${GUIPP_CXX_FLAGS})
add_link_options(${GUIPP_EXE_LINKER_FLAGS})

message(STATUS "app GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
message(STATUS "app GUIPP_EXE_LINKER_FLAGS: ${GUIPP_EXE_LINKER_FLAGS}")
#message(STATUS "app GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
message(STATUS "app GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")
message(STATUS "app GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")


# add the binary tree to the search path for include files
# so that we will find Version.h
include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS})

enable_testing()

set (TESTING_CXX_STANDARD ${GUIPP_CXX_STANDARD})
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

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} testing testlib ${GUIPP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
    set_target_properties(${test} PROPERTIES
                          FOLDER tests
                          CXX_STANDARD ${GUIPP_CXX_STANDARD})
endforeach(test)

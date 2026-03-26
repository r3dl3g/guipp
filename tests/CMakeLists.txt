cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("guipp-test" CXX)

set(BUILD_FOR_ARM OFF CACHE BOOL "On to build for arm architecture, Off for host architecture. default Off")

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)

if(NOT ANDROID AND NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if (GUIPP_USE_QT6 AND NOT ANDROID)
  DebugPrint("GUIPP test: build with Qt6!")
  find_package(Qt6Gui REQUIRED)
elseif (GUIPP_USE_QT5 AND NOT ANDROID)
  DebugPrint("GUIPP test: build with Qt5!")
  find_package(Qt5Gui REQUIRED)
endif()

if (GUIPP_USE_BOOST)
  find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)
endif()

set (TESTING_CXX_STANDARD ${GUIPP_CXX_STANDARD})

add_subdirectory(./../libs/testlib testlib)

if(GUIPP_BUILD_DEPENDENT_LIBS)
  add_subdirectory(${GUIPP_LIBS_DIR}/logging logging)
  add_subdirectory(${GUIPP_LIBS_DIR}/util util)
  add_subdirectory(./../libs/testing testing)
else ()
  set(logging_DIR ${CMAKE_PREFIX_PATH}/lib/cmake/logging)
  set(util_DIR ${CMAKE_PREFIX_PATH}/lib/cmake/util)
  set(testing_DIR ${CMAKE_PREFIX_PATH}/lib/cmake/testing)
  find_package(logging REQUIRED)
  find_package(util REQUIRED)
  find_package(testing REQUIRED)
endif()

DebugPrint("app GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
DebugPrint("app GUIPP_EXE_LINKER_FLAGS: ${GUIPP_EXE_LINKER_FLAGS}")
#DebugPrint("app GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
DebugPrint("app GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")
DebugPrint("app GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")


# add the binary tree to the search path for include files
# so that we will find Version.h
include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS} ${testlib_Include} ${TESTING_INCLUDE_DIRS})

enable_testing()

set(tests
    datamap_test
    diagram_test
    pixmap_test
    drawer_test
    icon_test
    stretch_test
    frames_test
)

DebugPrint("GUIPP_LIBRARIES: ${GUIPP_LIBRARIES}")

add_definitions(${GUIPP_CXX_FLAGS})
add_link_options(${GUIPP_EXE_LINKER_FLAGS})

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${TESTING_LIBRARIES} testlib ${GUIPP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    add_test(${test} ${CMAKE_CURRENT_BINARY_DIR}/${test})
    set_target_properties(${test} PROPERTIES
                          FOLDER tests
                          CXX_STANDARD ${GUIPP_CXX_STANDARD})
  # if (EMSCRIPTEN)
  #   set_target_properties(${test} PROPERTIES
  #                         LINK_FLAGS ${GUIPP_EXE_LINKER_FLAGS}
  #                         )
  # endif()
endforeach(test)

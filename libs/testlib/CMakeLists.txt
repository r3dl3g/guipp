cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set (testlib_Include "${CMAKE_CURRENT_SOURCE_DIR}")
set (testlib_Include ${testlib_Include} PARENT_SCOPE)

if(NOT TARGET testlib)

  add_subdirectory(${CMAKE_SOURCE_DIR}/gui gui)
  add_subdirectory(${CMAKE_SOURCE_DIR}/libs/testing testing)

  include_directories(${testlib_Include} ${GUIPP_INCLUDE_DIRS})

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h")

  add_definitions(${GUIPP_CXX_FLAGS})
  SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${GUIPP_EXE_LINKER_FLAGS})

  add_library(testlib STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
  set_target_properties(testlib PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        CXX_STANDARD ${GUIPP_CXX_STANDARD})
endif()

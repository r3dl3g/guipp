cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set (testlib_Include "${CMAKE_CURRENT_SOURCE_DIR}")
set (testlib_Include ${testlib_Include} PARENT_SCOPE)

if(NOT TARGET testlib)
  include_directories(${testlib_Include})
  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h")
  add_library(testlib ${SOURCE_FILES} ${INCLUDE_FILES})
  set_target_properties(testlib PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        CXX_STANDARD 11)
endif()

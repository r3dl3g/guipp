cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(NOT TARGET testlib)

  add_subdirectory(${CMAKE_SOURCE_DIR}/gui gui)

  if(GUIPP_BUILD_DEPENDENT_LIBS)
    add_subdirectory(./testing testing)
  else()
    find_package(testing REQUIRED)
  endif()

  set (testlib_Include "${CMAKE_CURRENT_SOURCE_DIR}")
  set (testlib_Include ${testlib_Include} PARENT_SCOPE)

  include_directories(${testlib_Include} ${GUIPP_INCLUDE_DIRS} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h")

  add_definitions(${GUIPP_CXX_FLAGS})
  SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "${GUIPP_EXE_LINKER_FLAGS}")

  add_library(testlib STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
  set_target_properties(testlib PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        CXX_STANDARD ${GUIPP_CXX_STANDARD})
endif()

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("layout" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../win win)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES
      attach_layout.cpp
      layout.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  set(LINKED_LIBRARIES
      guipp::core
      guipp::win
      ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_LAYOUT "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")

  if (GUIPP_STANDALONE_BUILD)
    configure_guipp_subpackage_config(guipp ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()

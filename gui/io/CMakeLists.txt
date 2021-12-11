cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../draw draw)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES
      pnm.cpp
      pnm_to_src.cpp
      wavelength_to_rgb.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  set(LINKED_LIBRARIES
      guipp::draw
      guipp::core)

  set(LINKED_SYS_LIBRARIES
      ${GUIPP_SYS_LIBRARIES})

  define_guipp_subpackage(${PROJECT_NAME} GUIPP_IO "${SOURCE_FILES}" "${INCLUDE_FILES}" "${LINKED_LIBRARIES}" "${LINKED_SYS_LIBRARIES}")
  configure_guipp_subpackage_config(guipp ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in "${INCLUDE_FILES}" "${LINKED_LIBRARIES}")

endif()

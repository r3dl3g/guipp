cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

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

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_IO
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::draw guipp::core ${GUIPP_SYS_LIBRARIES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           SUBDIR ${PROJECT_NAME}
                           CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                           FILES ${INCLUDE_FILES})

endif()

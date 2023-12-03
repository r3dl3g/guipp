cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("app" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(./../core core)

  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES app.cpp)
  set(INCLUDE_FILES app.h)

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_APP
                          STATIC
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           EXPORT_LIBS ON
                           SUBDIR ${PROJECT_NAME}
                           CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                           FILES ${INCLUDE_FILES})

endif()

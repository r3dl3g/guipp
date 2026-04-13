cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("layout" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../win win)

  set(SOURCE_FILES
      attach_layout.cpp
      layout.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_LAYOUT
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::core guipp::win ${GUIPP_SYS_LIBRARIES})

  install_guipp_subpackage(${PROJECT_NAME}
                           PREFIX guipp
                           SUBDIR ${PROJECT_NAME}
                           FILES ${INCLUDE_FILES})

endif()

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-app" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-app)

  add_subdirectory(./../core gui_core)

  include_directories(${GUIPP_INCLUDE_DIRS})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/app)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  set(SOURCE_FILES app.cpp)
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_definitions(${GUIPP_CXX_FLAGS})

  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.5 AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_library(gui++-app OBJECT ${SOURCE_FILES})
  else()
    add_library(gui++-app ${GUIPP_LINK} ${SOURCE_FILES})
  endif()

  set_target_properties(gui++-app PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
  )

  if (GUIPP_STANDALONE_BUILD)
    install(TARGETS gui++-app DESTINATION ${LIBRARY_INSTALL_DIR})

    install(FILES ${INCLUDE_FILES}
      DESTINATION ${INCLUDE_INSTALL_DIR})

  endif()

endif()

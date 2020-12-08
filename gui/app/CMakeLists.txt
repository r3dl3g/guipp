cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-app" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-app-obj)

  add_subdirectory(./../core gui_core)

  include_directories(${GUIPP_INCLUDE_DIRS})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/app)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_definitions(${GUIPP_CXX_FLAGS})

  add_library(gui++-app-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-app-obj PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        CXX_STANDARD 11
  )

  add_library(gui++-app STATIC $<TARGET_OBJECTS:gui++-app-obj>)

  if (GUIPP_STANDALONE_BUILD)
    install(TARGETS gui++-app DESTINATION ${LIBRARY_INSTALL_DIR})

    install(FILES ${INCLUDE_FILES}
      DESTINATION ${INCLUDE_INSTALL_DIR})

  endif()

endif()

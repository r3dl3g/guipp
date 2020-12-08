cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-draw-obj)

  add_subdirectory(../core gui_core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/draw)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(gui++-draw-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-draw-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    CXX_STANDARD 11
  )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-draw-static STATIC $<TARGET_OBJECTS:gui++-draw-obj>)
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-draw-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___draw_obj_EXPORTS)
    add_library(gui++-draw SHARED $<TARGET_OBJECTS:gui++-draw-obj>)
    target_link_libraries(gui++-draw PRIVATE gui++-core ${GUIPP_SYS_LIBRARIES})
    set_target_properties(gui++-draw PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER libraries
      VERSION ${GUIPP_VERSION}
      SOVERSION ${GUIPP_VERSION_MAJOR}
      CXX_STANDARD 11
      )
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-draw DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  GENERATE_EXPORT_HEADER(gui++-draw-obj
    BASE_NAME GUIPP_DRAW
    EXPORT_MACRO_NAME GUIPP_DRAW_EXPORT
    EXPORT_FILE_NAME gui++-draw-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-draw-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()

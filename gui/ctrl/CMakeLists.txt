cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-ctrl-obj)

  add_subdirectory(../win gui_win)
  add_subdirectory(../io gui_io)

  set(gui_include ${gui_include} ${CMAKE_CURRENT_BINARY_DIR})
  set(gui_include ${gui_include} PARENT_SCOPE)
  include_directories(${gui_include})

  if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
  endif (WIN32)

  add_definitions(${gui_defines})

  message(STATUS "ctrl gui_defines: ${gui_defines}")
  message(STATUS "ctrl gui_include: ${gui_include}")

  set (INCLUDE_INSTALL_DIR include/guipp/gui/ctrl)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(gui++-ctrl-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-ctrl-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-ctrl-static STATIC $<TARGET_OBJECTS:gui++-ctrl-obj>)
    install(TARGETS gui++-ctrl-static DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___ctrl_obj_EXPORTS)
    add_library(gui++-ctrl SHARED $<TARGET_OBJECTS:gui++-ctrl-obj>)
    target_link_libraries(gui++-ctrl PRIVATE gui++-core gui++-win gui++-draw gui++-io ${gui_sys_libraries})
    set_target_properties(gui++-ctrl PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      VERSION ${gui_VERSION}
      SOVERSION ${gui_VERSION_MAJOR})
    install(TARGETS gui++-ctrl DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  GENERATE_EXPORT_HEADER(gui++-ctrl-obj
    BASE_NAME GUIPP_CTRL
    EXPORT_MACRO_NAME GUIPP_CTRL_EXPORT
    EXPORT_FILE_NAME gui++-ctrl-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  install(FILES ${INCLUDE_FILES}
    ${PROJECT_BINARY_DIR}/gui++-ctrl-export.h
    DESTINATION ${INCLUDE_INSTALL_DIR})

endif()

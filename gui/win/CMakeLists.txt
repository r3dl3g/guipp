cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-win-obj)

  add_subdirectory(../core gui_core)

  set(gui_include ${gui_include} ${CMAKE_CURRENT_BINARY_DIR})
  set(gui_include ${gui_include} PARENT_SCOPE)
  include_directories(${gui_include})

  if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
  endif (WIN32)

  add_definitions(${gui_defines})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/win)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(gui++-win-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-win-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-win-static STATIC $<TARGET_OBJECTS:gui++-win-obj>)
    install(TARGETS gui++-win-static DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___win_obj_EXPORTS)
    add_library(gui++-win SHARED $<TARGET_OBJECTS:gui++-win-obj>)
    target_link_libraries(gui++-win PRIVATE gui++-core ${gui_sys_libraries})
    set_target_properties(gui++-win PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      VERSION ${gui_VERSION}
      SOVERSION ${gui_VERSION_MAJOR})

    install(TARGETS gui++-win DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  GENERATE_EXPORT_HEADER(gui++-win-obj
    BASE_NAME GUIPP_WIN
    EXPORT_MACRO_NAME GUIPP_WIN_EXPORT
    EXPORT_FILE_NAME gui++-win-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  install(FILES ${INCLUDE_FILES}
    ${PROJECT_BINARY_DIR}/gui++-win-export.h
    DESTINATION ${INCLUDE_INSTALL_DIR})

endif()

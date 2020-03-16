cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-layout" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-layout-obj)

  add_subdirectory(../win gui_win)

  set(gui_include ${gui_include} ${CMAKE_CURRENT_BINARY_DIR})
  set(gui_include ${gui_include} PARENT_SCOPE)
  include_directories(${gui_include})

  if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
  endif (WIN32)

  add_definitions(${gui_defines})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/layout)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  set(SOURCE_FILES attach_layout.cpp layout.cpp)
  set(INCLUDE_FILES
    adaption_layout.h
    attach_layout.h
    border_layout.h
    dynamic_border_layout.h
    grid_layout.h
    layout_container.h
    layout.h
    lineup_layout.h
    weighted_layout.h
    adaption_layout.inl
    attach_layout.inl
    border_layout.inl
    dynamic_border_layout.inl
    grid_layout.inl
    layout_container.inl
    layout.inl
    lineup_layout.inl
    weighted_layout.inl
  )

  add_library(gui++-layout-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-layout-obj PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries)

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-layout-static STATIC $<TARGET_OBJECTS:gui++-layout-obj>)
    install(TARGETS gui++-layout-static DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___layout_obj_EXPORTS)
    add_library(gui++-layout SHARED $<TARGET_OBJECTS:gui++-layout-obj>)
    target_link_libraries(gui++-layout PRIVATE gui++-core gui++-win ${gui_sys_libraries})
    set_target_properties(gui++-layout PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries
                          VERSION ${gui_VERSION}
                          SOVERSION ${gui_VERSION_MAJOR})
    install(TARGETS gui++-layout DESTINATION ${LIBRARY_INSTALL_DIR})
  endif()

  GENERATE_EXPORT_HEADER(gui++-layout-obj
    BASE_NAME GUIPP_LAYOUT
    EXPORT_MACRO_NAME GUIPP_LAYOUT_EXPORT
    EXPORT_FILE_NAME gui++-layout-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  install(FILES ${INCLUDE_FILES}
    ${PROJECT_BINARY_DIR}/gui++-layout-export.h
    DESTINATION ${INCLUDE_INSTALL_DIR})

endif()

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-layout" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-layout-obj)

  add_subdirectory(../win gui_win)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  if (WIN32)
    set(GUIPP_CXX_FLAGS ${GUIPP_CXX_FLAGS} -DWIN32_LEAN_AND_MEAN)
  endif (WIN32)

  add_definitions(${GUIPP_CXX_FLAGS})

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
    FOLDER libraries
    CXX_STANDARD 11
    )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-layout-static STATIC $<TARGET_OBJECTS:gui++-layout-obj>)
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-layout-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___layout_obj_EXPORTS)
    add_library(gui++-layout SHARED $<TARGET_OBJECTS:gui++-layout-obj>)
    target_link_libraries(gui++-layout PRIVATE gui++-core gui++-win ${GUIPP_SYS_LIBRARIES})
    set_target_properties(gui++-layout PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER libraries
      VERSION ${GUIPP_VERSION}
      SOVERSION ${GUIPP_VERSION_MAJOR}
      CXX_STANDARD 11
      )
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-layout DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  GENERATE_EXPORT_HEADER(gui++-layout-obj
    BASE_NAME GUIPP_LAYOUT
    EXPORT_MACRO_NAME GUIPP_LAYOUT_EXPORT
    EXPORT_FILE_NAME gui++-layout-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-layout-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("layout" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../win win)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set(SOURCE_FILES
      attach_layout.cpp
      layout.cpp
  )
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

  add_library(${PROJECT_NAME} ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "libgui++")

  set_target_properties(${PROJECT_NAME} PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___${PROJECT_NAME}_EXPORTS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
                          gui++::core
                          gui++::win
                          ${GUIPP_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(${PROJECT_NAME}
    BASE_NAME GUIPP_LAYOUT
    EXPORT_MACRO_NAME GUIPP_LAYOUT_EXPORT
    EXPORT_FILE_NAME gui++-${PROJECT_NAME}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    write_subpackage_config(gui++ ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("draw" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../core core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set(SOURCE_FILES
      arc_coords.cpp
      bitmap_info.cpp
      datamap.cpp
      converter.cpp
      frames.cpp
      drawers.cpp
      drawers_qt.cpp
      drawers_win32.cpp
      drawers_x11.cpp
      brush.cpp
      diagram.cpp
      bitmap.cpp
      font.cpp
      graphics.cpp
      image_data.cpp
      pen.cpp
      text_origin.cpp
      use.cpp
      icons.cpp
      clock.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(${PROJECT_NAME} ${GUIPP_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(gui++::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "libgui++")

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)

    add_definitions(-DGUIPP_BUILT_AS_STATIC)

  elseif(GUIPP_BUILD_SHARED_MODULE_LIBS)

    add_definitions(-Dgui___${PROJECT_NAME}_EXPORTS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
                          gui++::core
                          ${UTIL_LIBRARIES}
                          ${GUIPP_SYS_LIBRARIES})

  endif()

  set_target_properties(${PROJECT_NAME} PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )

  GENERATE_EXPORT_HEADER(${PROJECT_NAME}
    BASE_NAME GUIPP_DRAW
    EXPORT_MACRO_NAME GUIPP_DRAW_EXPORT
    EXPORT_FILE_NAME gui++-${PROJECT_NAME}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    write_subpackage_config(gui++ ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in "${INCLUDE_FILES}")
  endif()

endif()

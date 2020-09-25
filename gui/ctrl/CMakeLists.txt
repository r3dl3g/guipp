cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-ctrl" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-ctrl-obj)

  add_subdirectory(../win gui_win)
  add_subdirectory(../io gui_io)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
  endif (WIN32)

  add_definitions(${GUIPP_CXX_FLAGS})

  message(STATUS "ctrl GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
  message(STATUS "ctrl GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")

  set (INCLUDE_INSTALL_DIR include/guipp/gui/ctrl)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  file(GLOB RESOURCE_FILES "res/*.h")

  add_library(gui++-ctrl-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES})

  set_target_properties(gui++-ctrl-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    CXX_STANDARD 11
    )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-ctrl-static STATIC $<TARGET_OBJECTS:gui++-ctrl-obj>)
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-ctrl-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___ctrl_obj_EXPORTS)
    add_library(gui++-ctrl SHARED $<TARGET_OBJECTS:gui++-ctrl-obj>)
    target_link_libraries(gui++-ctrl PRIVATE gui++-core gui++-win gui++-draw gui++-io ${GUIPP_SYS_LIBRARIES})
    set_target_properties(gui++-ctrl PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER libraries
      VERSION ${GUIPP_VERSION}
      SOVERSION ${GUIPP_VERSION_MAJOR}
      CXX_STANDARD 11
      )
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-ctrl DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  GENERATE_EXPORT_HEADER(gui++-ctrl-obj
    BASE_NAME GUIPP_CTRL
    EXPORT_MACRO_NAME GUIPP_CTRL_EXPORT
    EXPORT_FILE_NAME gui++-ctrl-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-ctrl-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})

    install(FILES ${RESOURCE_FILES}
      DESTINATION ${INCLUDE_INSTALL_DIR}/res)
  endif()

endif()

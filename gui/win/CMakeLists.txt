cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-win" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-win-obj)

  add_subdirectory(../core gui_core)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/win)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_library(gui++-win-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-win-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    CXX_STANDARD 11
  )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-win-static STATIC $<TARGET_OBJECTS:gui++-win-obj>)
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-win-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___win_obj_EXPORTS)
    add_library(gui++-win SHARED $<TARGET_OBJECTS:gui++-win-obj>)
    target_link_libraries(gui++-win PRIVATE gui++-core ${GUIPP_SYS_LIBRARIES})
    set_target_properties(gui++-win PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER libraries
      VERSION ${GUIPP_VERSION}
      SOVERSION ${GUIPP_VERSION_MAJOR}
      CXX_STANDARD 11
    )

    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-win DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  GENERATE_EXPORT_HEADER(gui++-win-obj
    BASE_NAME GUIPP_WIN
    EXPORT_MACRO_NAME GUIPP_WIN_EXPORT
    EXPORT_FILE_NAME gui++-win-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
  )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-win-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()

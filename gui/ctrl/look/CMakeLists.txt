cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-look" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-look-obj)

  add_subdirectory(../../draw gui_draw)
  add_subdirectory(../../io gui_io)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  add_definitions(${GUIPP_CXX_FLAGS})

  message(STATUS "look GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#  message(STATUS "look GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")

  set (INCLUDE_INSTALL_DIR include/guipp/gui/look)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")
  file(GLOB RESOURCE_FILES "res/*.h")

  add_library(gui++-look-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES} ${RESOURCE_FILES})

  set_target_properties(gui++-look-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    CXX_STANDARD 11
  )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-look-static STATIC $<TARGET_OBJECTS:gui++-look-obj>)
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-look-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___look_obj_EXPORTS)
    add_library(gui++-look SHARED $<TARGET_OBJECTS:gui++-look-obj>)
    target_link_libraries(gui++-look PRIVATE gui++-core gui++-draw gui++-io ${UTIL_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
    set_target_properties(gui++-look PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER libraries
      VERSION ${GUIPP_VERSION}
      SOVERSION ${GUIPP_VERSION_MAJOR}
      CXX_STANDARD 11
      )
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-look DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  GENERATE_EXPORT_HEADER(gui++-look-obj
    BASE_NAME GUIPP_LOOK
    EXPORT_MACRO_NAME GUIPP_LOOK_EXPORT
    EXPORT_FILE_NAME gui++-look-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-look-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})

    install(FILES ${RESOURCE_FILES}
      DESTINATION ${INCLUDE_INSTALL_DIR}/res)
  endif()

endif()

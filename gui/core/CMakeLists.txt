cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("gui++-core" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET gui++-core-obj)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  set (INCLUDE_INSTALL_DIR include/guipp/gui/core)
  set (LIBRARY_INSTALL_DIR lib/guipp)

  file(GLOB SOURCE_FILES "*.cpp")
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  message(STATUS "GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")

  add_definitions(-DEXPERIMENTAL ${GUIPP_CXX_FLAGS})

  add_library(gui++-core-obj OBJECT ${SOURCE_FILES} ${INCLUDE_FILES})

  set_target_properties(gui++-core-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    CXX_STANDARD 11
    )

  if(GUIPP_BUILD_STATIC_MODULE_LIBS)
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(gui++-core-static STATIC $<TARGET_OBJECTS:gui++-core-obj>)
    target_link_libraries(gui++-core-static PRIVATE ${LOGGING_LIBRARIES})

    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-core-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  if(GUIPP_BUILD_SHARED_MODULE_LIBS)
    add_definitions(-Dgui___core_obj_EXPORTS)
    add_library(gui++-core SHARED $<TARGET_OBJECTS:gui++-core-obj>)
    target_link_libraries(gui++-core PRIVATE ${LOGGING_LIBRARIES} ${GUIPP_SYS_LIBRARIES} ${LOGGING_SYS_LIBRARIES})
    set_target_properties(gui++-core PROPERTIES
      POSITION_INDEPENDENT_CODE ON
      FOLDER libraries
      VERSION ${GUIPP_VERSION}
      SOVERSION ${GUIPP_VERSION_MAJOR}
      CXX_STANDARD 11
      )
    if (GUIPP_STANDALONE_BUILD)
      install(TARGETS gui++-core DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()
  endif()

  GENERATE_EXPORT_HEADER(gui++-core-obj
    BASE_NAME GUIPP_CORE
    EXPORT_MACRO_NAME GUIPP_CORE_EXPORT
    EXPORT_FILE_NAME gui++-core-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

  if (GUIPP_STANDALONE_BUILD)
    install(FILES ${INCLUDE_FILES}
      ${PROJECT_BINARY_DIR}/gui++-core-export.h
      DESTINATION ${INCLUDE_INSTALL_DIR})
  endif()

endif()

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include (GenerateExportHeader)
include (CMakePackageConfigHelpers)
include (GNUInstallDirs)
include (CMakeParseArguments)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

function(DebugPrint MSG)
  if(VERBOSE)
    message(STATUS "${MSG}")
  endif()
endfunction(DebugPrint)

function(install_guipp_subpackage TARGET)

  if(GUIPP_CONFIG_INSTALL)
    cmake_parse_arguments(
        ARG           # prefix
        "EXPORT_LIBS" # boolean arguments
        "PREFIX;CONFIG;SUBDIR;NAMESPACE" # mono-valued arguments
        "FILES"       # multi-valued arguments
        ${ARGN}       # num of arguments of the function to parse
    )
    if((NOT TARGET) OR (NOT ARG_CONFIG) OR (NOT ARG_SUBDIR))
        message(FATAL_ERROR "You must provide target, subdir and config")
    endif()
    if((NOT ARG_PREFIX) AND (NOT ARG_NAMESPACE))
        message(FATAL_ERROR "You must provide either prefix or namespace")
    endif()
    if(NOT ARG_NAMESPACE)
      set(ARG_NAMESPACE ${ARG_PREFIX})
    endif()

    set(${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/gui/${ARG_SUBDIR})
    set(${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${ARG_NAMESPACE})

    target_include_directories(${TARGET}
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR}>")

    configure_package_config_file(
        ${ARG_CONFIG}
        ${CMAKE_CURRENT_BINARY_DIR}/${ARG_PREFIX}${TARGET}Config.cmake
        INSTALL_DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR}
        PATH_VARS CMAKE_INSTALL_INCLUDEDIR
    )

    if(ARG_EXPORT_LIBS OR GUIPP_BUILD_STATIC_MODULE_LIBS OR GUIPP_BUILD_SHARED_MODULE_LIBS)
      install(
          TARGETS ${TARGET}
          EXPORT ${ARG_PREFIX}${TARGET}_Targets
          OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
          INCLUDES DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR}
      )

      install(
          EXPORT ${ARG_PREFIX}${TARGET}_Targets
          NAMESPACE ${ARG_NAMESPACE}::
          FILE ${ARG_PREFIX}${TARGET}Targets.cmake
          DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR}
      )

      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${ARG_PREFIX}${TARGET}Config.cmake
          DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_CMAKEDIR}
      )

      export(TARGETS ${TARGET} FILE ${ARG_PREFIX}${TARGET}Targets.cmake)

    endif()

    if(EXISTS "${PROJECT_BINARY_DIR}/gui++-${TARGET}-export.h")
      list(APPEND ARG_FILES "${PROJECT_BINARY_DIR}/gui++-${TARGET}-export.h")
    endif()

    install(FILES ${ARG_FILES}
        DESTINATION ${${ARG_PREFIX}${TARGET}_INSTALL_INCLUDEDIR}
    )

  endif()

endfunction(install_guipp_subpackage)

function(define_guipp_subpackage TARGET)

  cmake_parse_arguments(
    ARG             # prefix
    "STATIC;SHARED" # boolean arguments
    "BASE;PREFIX;VERSION;VERSION_MAJOR;CXX_STANDARD;LINK" # mono-valued arguments
    "FLAGS;SOURCES;LIBS"    # multi-valued arguments
    ${ARGN}         # num of arguments to parse
  )
  if(NOT TARGET)
    message(FATAL_ERROR "You must provide TARGET")
  endif()
  if(NOT ARG_BASE)
    message(FATAL_ERROR "You must provide BASE")
  endif()
  if(NOT ARG_FLAGS)
    set(ARG_FLAGS ${GUIPP_CXX_FLAGS})
  endif()
  if(NOT ARG_PREFIX)
    set(ARG_PREFIX libguipp)
  endif()
  if(NOT ARG_LINK)
    set(ARG_LINK ${GUIPP_LINK})
  endif()
  if(NOT ARG_VERSION)
    set(ARG_VERSION ${GUIPP_VERSION})
  endif()
  if(NOT ARG_VERSION_MAJOR)
    set(ARG_VERSION_MAJOR ${GUIPP_VERSION_MAJOR})
  endif()
  if(NOT ARG_CXX_STANDARD)
    set(ARG_CXX_STANDARD ${GUIPP_CXX_STANDARD})
  endif()

  add_definitions(${ARG_FLAGS})

  if(ARG_STATIC OR (NOT ARG_SHARED AND GUIPP_BUILD_STATIC_MODULE_LIBS))
    message(STATUS "Build " ${TARGET} " as static library ")
    add_definitions(-DGUIPP_BUILT_AS_STATIC)
    add_library(${TARGET} STATIC ${ARG_SOURCES})
  elseif(ARG_SHARED OR (NOT ARG_STATIC AND GUIPP_BUILD_SHARED_MODULE_LIBS))
    message(STATUS "Build " ${TARGET} " as shared library ")
    add_definitions(-Dgui___${TARGET}_EXPORTS)
    add_library(${TARGET} SHARED ${ARG_SOURCES})
    target_link_libraries(${TARGET} PRIVATE ${ARG_LIBS})
  else()
    message(STATUS "Build " ${TARGET} " as " ${ARG_LINK} " library ")
    add_library(${TARGET} ${ARG_LINK} ${ARG_SOURCES})
  endif()

  add_library(guipp::${TARGET} ALIAS ${TARGET})
  set_target_properties(${TARGET} PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        PREFIX ${ARG_PREFIX}
                        VERSION ${ARG_VERSION}
                        SOVERSION ${ARG_VERSION_MAJOR}
                        CXX_STANDARD ${ARG_CXX_STANDARD})

  GENERATE_EXPORT_HEADER(${TARGET}
    BASE_NAME ${ARG_BASE}
    EXPORT_MACRO_NAME ${ARG_BASE}_EXPORT
    EXPORT_FILE_NAME gui++-${TARGET}-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC)

endfunction(define_guipp_subpackage)

function(guipp_app TARGET)

  cmake_parse_arguments(
    ARG                             # prefix
    ""                              # boolean arguments
    "PACKAGE_NAME;MAIN_SRC;BIN_DIR" # mono-valued arguments
    "INCLUDES;SOURCES;LIBS"                  # multi-valued arguments
    ${ARGN}                         # num of arguments to parse
  )

  if(NOT TARGET)
    message(FATAL_ERROR "You must provide TARGET")
  endif()
  if(NOT ARG_MAIN_SRC)
    set(ARG_MAIN_SRC ${TARGET}.cpp)
  endif()
  if(NOT ARG_PACKAGE_NAME)
    set(ARG_PACKAGE_NAME "org.organization.${TARGET}")
  endif()
  if(NOT ARG_BIN_DIR)
    set(ARG_BIN_DIR bin)
  endif()

  include_directories(${GUIPP_INCLUDE_DIRS} ${ARG_INCLUDES})
  add_definitions(${GUIPP_CXX_FLAGS})

  if (ANDROID)
    add_library(${TARGET} SHARED ${ARG_MAIN_SRC} ${ARG_SOURCES})
  elseif (EMSCRIPTEN)
    add_executable(${TARGET} ${ARG_MAIN_SRC} ${ARG_SOURCES})
  else ()
    add_executable(${TARGET} WIN32 ${ARG_MAIN_SRC} ${ARG_SOURCES})
  endif ()

  target_link_libraries(${TARGET} ${ARG_LIBS} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
  install(TARGETS ${TARGET} DESTINATION ${ARG_BIN_DIR})
  set_target_properties(${TARGET} PROPERTIES
                        FOLDER apps
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )
  if (EMSCRIPTEN)
    set_target_properties(${TARGET} PROPERTIES
                          LINK_FLAGS "-s USE_PTHREADS=1")
  endif()
  if (ANDROID)
    add_qt_android_apk(${TARGET}_apk ${TARGET}
                         PACKAGE_NAME ${ARG_PACKAGE_NAME}
                         INSTALL)
  endif()

endfunction(guipp_app)

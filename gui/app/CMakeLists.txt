cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("app" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(./../core core)

  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES app.cpp)
  set(INCLUDE_FILES app.h)

  add_definitions(${GUIPP_CXX_FLAGS})

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.5 AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(GUIPP_APP_USE_OBJECT TRUE)
    add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})
  else()
    set(GUIPP_APP_USE_OBJECT FALSE)
    add_library(${PROJECT_NAME} ${GUIPP_LINK} ${SOURCE_FILES})
  endif()

  add_library(guipp::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
  set_target_properties(${PROJECT_NAME} PROPERTIES
                        POSITION_INDEPENDENT_CODE ON
                        FOLDER libraries
                        PREFIX "libguipp"
                        VERSION ${GUIPP_VERSION}
                        SOVERSION ${GUIPP_VERSION_MAJOR}
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
  )

  GENERATE_EXPORT_HEADER(${PROJECT_NAME}
    BASE_NAME APP
    EXPORT_MACRO_NAME APP_EXPORT
    EXPORT_FILE_NAME gui++-app-export.h
    STATIC_DEFINE GUIPP_BUILT_AS_STATIC)

  configure_guipp_subpackage_config(${PROJECT_NAME}
                                    PREFIX guipp
                                    SUBDIR ${PROJECT_NAME}
                                    CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                                    EXPORT_LIBS ${GUIPP_APP_USE_OBJECT}
                                    INCLUDES ${INCLUDE_FILES})

endif()

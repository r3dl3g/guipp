cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("io" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(../draw draw)

  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  set(GUIPP_INCLUDE_DIRS ${GUIPP_INCLUDE_DIRS} PARENT_SCOPE)
  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES
      pnm.cpp
      pnm_to_src.cpp
      wavelength_to_rgb.cpp
  )
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  define_guipp_subpackage(${PROJECT_NAME}
                          BASE GUIPP_IO
                          PREFIX libguipp
                          LINK ${GUIPP_LINK}
                          VERSION ${GUIPP_VERSION}
                          VERSION_MAJOR ${GUIPP_VERSION_MAJOR}
                          CXX_STANDARD ${GUIPP_CXX_STANDARD}
                          SOURCES ${SOURCE_FILES} ${INCLUDE_FILES}
                          LIBS guipp::draw guipp::core ${GUIPP_SYS_LIBRARIES})

  configure_guipp_subpackage_config(${PROJECT_NAME}
                                    PREFIX guipp
                                    SUBDIR ${PROJECT_NAME}
                                    CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/../SubConfig.cmake.in
                                    INCLUDES ${INCLUDE_FILES})

endif()

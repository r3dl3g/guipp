cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("app" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

if(NOT TARGET ${PROJECT_NAME})

  add_subdirectory(./../core core)

  include_directories(${GUIPP_INCLUDE_DIRS})

  set(SOURCE_FILES app.cpp)
  file(GLOB INCLUDE_FILES "*.h" "*.inl")

  add_definitions(${GUIPP_CXX_FLAGS})

  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.5 AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set (GUIPP_APP_USE_OBJECT TRUE)
    add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})
  else()
    set (GUIPP_APP_USE_OBJECT FALSE)
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

  if (GUIPP_STANDALONE_BUILD)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/guipp/app)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/guipp)

    target_include_directories(${PROJECT_NAME}
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
    )

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../Config.cmake.in
      "${CMAKE_CURRENT_BINARY_DIR}/guippappConfig.cmake"
      INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    if (NOT GUIPP_APP_USE_OBJECT)
      install(TARGETS ${PROJECT_NAME}
              EXPORT guippappTargets
              LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
              ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
              RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
              INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
      )
    else()
      install(TARGETS ${PROJECT_NAME}
              EXPORT guippappTargets
              OBJECTS DESTINATION ${LIBRARY_INSTALL_DIR}
              INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
      )
    endif()

    install(FILES ${INCLUDE_FILES}
            DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(EXPORT guippappTargets
            FILE guippappTargets.cmake
            NAMESPACE guipp::
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/guippappConfig.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR}/cmake
    )

  endif()

endif()

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("gui++-raspi" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

if(NOT TARGET gui++-raspi-obj)

    add_subdirectory(../gui/core gui_core)

    set(gui_include ${gui_include} ${CMAKE_CURRENT_BINARY_DIR})
    set(gui_include ${gui_include} PARENT_SCOPE)
    include_directories(${gui_include} /opt/raspberrypi/rootfs/opt/vc/include /opt/vc/include)

    if (WIN32)
      add_definitions(-DWIN32_LEAN_AND_MEAN)
    endif (WIN32)

    add_definitions(${gui_defines})

if (BUILD_FOR_ARM)
    file(GLOB SOURCE_FILES "*.cpp")
    file(GLOB INCLUDE_FILES "*.h" "*.inl")
endif()

    add_library(gui++-raspi-obj OBJECT ${SOURCE_FILES})

    set_target_properties(gui++-raspi-obj PROPERTIES
                          POSITION_INDEPENDENT_CODE ON
                          FOLDER libraries)

    if(GUIPP_BUILD_STATIC_MODULE_LIBS)
        add_definitions(-DGUIPP_BUILT_AS_STATIC)
        add_library(gui++-raspi-static STATIC $<TARGET_OBJECTS:gui++-raspi-obj>)
        install(TARGETS gui++-raspi-static DESTINATION ${LIBRARY_INSTALL_DIR})
    endif()

    if(GUIPP_BUILD_SHARED_MODULE_LIBS)
        add_definitions(-Dgui___raspi_obj_EXPORTS)
        add_library(gui++-raspi SHARED $<TARGET_OBJECTS:gui++-raspi-obj>)
        target_link_libraries(gui++-raspi PRIVATE gui++-draw gui++-core ${gui_sys_libraries})
        set_target_properties(gui++-raspi PROPERTIES
                                       POSITION_INDEPENDENT_CODE ON
                                       FOLDER libraries
                                       VERSION ${gui_VERSION}
                                       SOVERSION ${gui_VERSION_MAJOR})
    endif()

    GENERATE_EXPORT_HEADER(gui++-raspi-obj
        BASE_NAME GUIPP_RASPI
        EXPORT_MACRO_NAME GUIPP_RASPI_EXPORT
        EXPORT_FILE_NAME gui++-raspi-export.h
        STATIC_DEFINE GUIPP_BUILT_AS_STATIC
    )

    install(FILES ${INCLUDE_FILES}
                  ${PROJECT_BINARY_DIR}/gui++-raspi-export.h
            DESTINATION ${INCLUDE_INSTALL_DIR})

endif()

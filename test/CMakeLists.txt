cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Gui-Test" CXX)

set(BUILD_FOR_ARM OFF CACHE BOOL "On to build for arm architecture, Off for host architecture. default Off")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

if (APPLE)
  add_definitions(-DCOCOA -std=c++11)
elseif (UNIX)
  add_definitions(-DX11 -std=c++11)
endif()

add_subdirectory(./../gui gui)
if (BUILD_FOR_ARM)
  add_subdirectory(./../raspi raspi)
endif()

add_definitions(${gui_defines})
message(STATUS "app gui_defines: ${gui_defines}")
message(STATUS "app gui_include: ${gui_include}")
message(STATUS "app gui_libraries: ${gui_libraries}")
message(STATUS "app gui_sys_libraries: ${gui_sys_libraries}")


# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR} ${gui_include})

enable_testing()

add_subdirectory(./../testlib testlib)
add_subdirectory(./../testing testing)

if (BUILD_FOR_ARM)
    add_definitions(-DBUILD_FOR_ARM)
    set (MMAL_LIBS mmal_core mmal_util mmal_vc_client bcm_host rt vcsm vchiq_arm vcos)
    include_directories(/opt/raspberrypi/rootfs/opt/vc/include /opt/vc/include)

# bcm_host
    set(apps
        raspicam_test
        spectrometer
        WindowTest
        BorderTest
        hello_world
        filemanager
        list_app
        tree_app
        new_border_layout
        attach_all_layout
        dialogs
    )
else()
    set(apps
        WindowTest
        BorderTest
        hello_world
        filemanager
        list_app
        tree_app
        new_border_layout
        attach_all_layout
        dialogs
        spectrometer
    )
endif()

set(tests
    datamap_test
    pixmap_test
    drawer_test
    icon_test
    stretch_test
    frames_test
)

message(STATUS "gui_libraries: ${gui_libraries}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    add_definitions(/Y- /bigobj)
endif()

if (BUILD_FOR_ARM)
    LINK_DIRECTORIES(/opt/raspberrypi/sysroot/opt/vc/lib)
    LINK_DIRECTORIES(/opt/vc/lib)
endif()

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} testing testlib ${gui_libraries} ${gui_sys_libraries})
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
    set_target_properties(${test} PROPERTIES FOLDER tests)
endforeach(test)

foreach(test ${apps})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${gui_app_libraries} ${gui_sys_libraries})
    install(TARGETS ${test} DESTINATION bin)
    set_target_properties(${test} PROPERTIES FOLDER apps)
endforeach(test)

if (BUILD_FOR_ARM)
    target_link_libraries(raspicam_test testlib ${raspipp_libraries} ${MMAL_LIBS})
    target_link_libraries(spectrometer testlib ${raspipp_libraries} ${MMAL_LIBS})
endif()

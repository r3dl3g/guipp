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

add_definitions(${gui_defines})
message(STATUS "app gui_defines: ${gui_defines}")
message(STATUS "app gui_include: ${gui_include}")
message(STATUS "app gui_libraries: ${gui_libraries}")
message(STATUS "app gui_sys_libraries: ${gui_sys_libraries}")


# add the binary tree to the search path for include files
# so that we will find Version.h
# "${PROJECT_BINARY_DIR}/../IT3DCameraLib"
include_directories(${PROJECT_BINARY_DIR}
                    ${gui_include}
                    /opt/raspberrypi/sysroot/opt/vc/include
)

enable_testing()

if (BUILD_FOR_ARM)
    add_definitions(-DBUILD_FOR_ARM)
    set (MMAL_LIBS mmal_core mmal_util mmal_vc_client rt vcsm vchiq_arm vcos)
# bcm_host
    set(tests
        raspicam_test
        spectrometer
    )
else()
    set(tests
        WindowTest
        BorderTest
        hello_world
        spectrometer
    )
endif()

message(STATUS "gui_libraries: ${gui_libraries}")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_definitions(/Y- /bigobj)
endif()

LINK_DIRECTORIES(/opt/raspberrypi/sysroot/opt/vc/lib)

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} ${gui_libraries} ${gui_sys_libraries})
    add_test(${test} ${CMAKE_BINARY_DIR}/bin/${test})
endforeach(test)

if (BUILD_FOR_ARM)
    target_link_libraries(raspicam_test gui++-raspi-static ${MMAL_LIBS})
    target_link_libraries(spectrometer gui++-raspi-static ${MMAL_LIBS})
endif()

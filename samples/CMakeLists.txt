cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Samples" CXX)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT ANDROID AND NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

add_subdirectory(./../gui gui)

if (GUIPP_USE_QT6 AND NOT ANDROID)
  DebugPrint("GUIPP samples: build with Qt6!")
  find_package(Qt6Gui REQUIRED)
elseif (GUIPP_USE_QT5 AND NOT ANDROID)
  DebugPrint("GUIPP samples: build with Qt5!")
  find_package(Qt5Gui REQUIRED)
endif()

if (GUIPP_USE_BOOST)
  find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/Y- /bigobj)
endif()

DebugPrint("samples GUIPP_CXX_FLAGS: ${GUIPP_CXX_FLAGS}")
#DebugPrint("samples GUIPP_INCLUDE_DIRS: ${GUIPP_INCLUDE_DIRS}")
DebugPrint("samples GUIPP_APP_LIBRARIES: ${GUIPP_APP_LIBRARIES}")
DebugPrint("samples GUIPP_SYS_LIBRARIES: ${GUIPP_SYS_LIBRARIES}")
DebugPrint("samples GUIPP_EXE_LINKER_FLAGS: ${GUIPP_EXE_LINKER_FLAGS}")


# add the binary tree to the search path for include files
# so that we will find Version.h
include_directories(${PROJECT_BINARY_DIR} ${GUIPP_INCLUDE_DIRS})

add_definitions(${GUIPP_CXX_FLAGS})

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
endif (EMSCRIPTEN)

if (${CMAKE_VERSION} VERSION_LESS "3.13.0")
  set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${GUIPP_EXE_LINKER_FLAGS})
else ()
  add_link_options(${GUIPP_EXE_LINKER_FLAGS})
endif ()

function(guipp_sample PROJECT_NAME)

  if (ANDROID)
    add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cpp)
  elseif (EMSCRIPTEN)
    add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
  else ()
    add_executable(${PROJECT_NAME} WIN32 ${PROJECT_NAME}.cpp)
  endif ()

  target_link_libraries(${PROJECT_NAME} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
  install(TARGETS ${PROJECT_NAME} DESTINATION bin)
  set_target_properties(${PROJECT_NAME} PROPERTIES
                        FOLDER apps
                        CXX_STANDARD ${GUIPP_CXX_STANDARD}
                        )
  if (EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          LINK_FLAGS "-s USE_PTHREADS=1")
  endif ()
  if (ANDROID)
    add_qt_android_apk(${PROJECT_NAME}_apk ${PROJECT_NAME}
                         PACKAGE_NAME "de.redspace.${PROJECT_NAME}"
                         INSTALL)
  endif ()

endfunction(guipp_sample)

add_subdirectory(AttachAllLayoutApp)
add_subdirectory(BorderLayoutApp)
add_subdirectory(ColumnListViewApp)
add_subdirectory(DiagramsApp)
add_subdirectory(DialogsApp)
add_subdirectory(DynamicBorderLayoutApp)
add_subdirectory(EditorApp)
add_subdirectory(FilemanagerApp)
add_subdirectory(HelloWorldApp)
add_subdirectory(IconsApp)
add_subdirectory(ListApp)
add_subdirectory(MultiInputApp)
add_subdirectory(ScrollViewApp)
add_subdirectory(SplitListViewApp)
add_subdirectory(SplitView2App)
add_subdirectory(SplitViewApp)
add_subdirectory(StackedViewApp)
add_subdirectory(TableApp)
add_subdirectory(TextApp)
add_subdirectory(TilesApp)
add_subdirectory(TreeApp)
add_subdirectory(UnevenListApp)
add_subdirectory(WeightedLayoutApp)

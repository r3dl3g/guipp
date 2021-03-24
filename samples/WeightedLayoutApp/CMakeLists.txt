cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("WeightedLayoutApp" CXX)

if (ANDROID)
  add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cpp)
else ()
  add_executable(${PROJECT_NAME} WIN32 ${PROJECT_NAME}.cpp)
endif ()

target_link_libraries(${PROJECT_NAME} ${GUIPP_APP_LIBRARIES} ${GUIPP_SYS_LIBRARIES})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
set_target_properties(${PROJECT_NAME} PROPERTIES
                      FOLDER apps
                      CXX_STANDARD ${GUIPP_CXX_STANDARD})

if (ANDROID)
  add_qt_android_apk(${PROJECT_NAME}_apk ${PROJECT_NAME}
                       PACKAGE_NAME "de.redspace.${PROJECT_NAME}"
                       INSTALL)
endif ()

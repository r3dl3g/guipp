. ./.project_defines

if [ "$PLATFORM" == "qt5" ]; then
    DEFINES="-DGUIPP_USE_QT5=ON"
elif [ "$PLATFORM" == "qt6" ]; then
    DEFINES="-DGUIPP_USE_QT6=ON"
elif [ "$PLATFORM" == "emc" ]; then
    INSTALL_PREFIX=~/.local/emc
    CMAKE_CMD="cmake --preset emc_debug"
elif [ "$PLATFORM" == "win32" ]; then
    INSTALL_PREFIX=~/.local/win32
    DEFINES="-DWIN32=ON -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON -DGUIPP_BUILD_SHARED_MODULE_LIBS=OFF"
# elif [ "$PLATFORM" == "x11" ]; then
fi

prepare Debug $@
prepare Release $@

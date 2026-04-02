. ./.project_defines

if [ "$PLATFORM" == "qt5" ]; then
    DEFINES="-DGUIPP_SAMPLES=ON -DGUIPP_USE_QT5=ON"
elif [ "$PLATFORM" == "qt6" ]; then
    DEFINES="-DGUIPP_SAMPLES=ON -DGUIPP_USE_QT6=ON"
elif [ "$PLATFORM" == "emc" ]; then
    INSTALL_PREFIX=~/.local/emc
    CMAKE_CMD="cmake --preset emc_debug"
    DEFINES=-DGUIPP_SAMPLES=ON
# elif [ "$PLATFORM" == "x11" ]; then
fi

prepare Debug $@
prepare Release $@

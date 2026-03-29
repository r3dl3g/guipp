. ./.project_defines

BUILD_PREFIX=~/.cache/build/emc
INSTALL_PREFIX=~/.local/emc
CMAKE_CMD="emcmake cmake"
DEFINES=-DGUIPP_SAMPLES=ON

prepare Debug $@
prepare Release $@

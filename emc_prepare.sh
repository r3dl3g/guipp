. ./.project_defines

BUILD_PREFIX=~/.cache/build/emc
INSTALL_PREFIX=~/.local/emc
CMAKE_CMD="emcmake cmake"

prepare Debug -DVERBOSE=ON -DGUIPP_TESTS=ON -DGUIPP_SAMPLES=ON $@
prepare Release $@

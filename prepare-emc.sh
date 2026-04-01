. ./.project_defines

PROJECT=guipp.emc
INSTALL_PREFIX=~/.local/emc
CMAKE_CMD="emcmake cmake"
DEFINES=-DGUIPP_SAMPLES=ON

prepare Debug $@
prepare Release $@

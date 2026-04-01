. ./.project_defines

PROJECT=guipp.emc
INSTALL_PREFIX=~/.local/emc
CMAKE_CMD="cmake --preset emc_debug"
DEFINES=-DGUIPP_SAMPLES=ON

prepare Debug $@
prepare Release $@

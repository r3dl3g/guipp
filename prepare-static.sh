. ./.project_defines

BUILD_PREFIX=~/.cache/build/static
INSTALL_PREFIX=~/.local/static
DEFINES="-DGUIPP_SAMPLES=ON -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON -DGUIPP_BUILD_SHARED_MODULE_LIBS=OFF"

prepare Debug -DVERBOSE=ON $@
prepare Release $@

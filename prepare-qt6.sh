. ./.project_defines

PROJECT=guipp.qt6
DEFINES="-DGUIPP_SAMPLES=ON -DGUIPP_USE_QT6=ON"

prepare Debug -DVERBOSE=ON $@
prepare Release $@

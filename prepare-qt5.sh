. ./.project_defines

DEFINES="-DGUIPP_SAMPLES=ON -DGUIPP_USE_QT5=ON"

prepare Debug -DVERBOSE=ON $@
prepare Release $@

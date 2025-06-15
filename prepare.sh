echo ======= New Prepare at $(date) ======= >> prepare.log
cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=../libs \
    -DCMAKE_PREFIX_PATH=../libs \
    -DGUIPP_TESTS=OFF \
    -DGUIPP_SAMPLES=OFF \
    -DGUIPP_CONFIG_INSTALL=ON \
    -DGUIPP_BUILD_DEPENDENT_LIBS=OFF \
    -DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF \
    -DGUIPP_BUILD_SHARED_MODULE_LIBS=OFF \
 $* 2>&1 | tee -a prepare.log

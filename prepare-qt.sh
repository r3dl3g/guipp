echo ======= New Prepare at $(date) ======= >> prepare.log
defines="-DCMAKE_INSTALL_PREFIX=../install
-DCMAKE_PREFIX_PATH=../install
-DGUIPP_TESTS=OFF
-DGUIPP_SAMPLES=ON
-DGUIPP_CONFIG_INSTALL=ON
-DGUIPP_USE_QT6=ON
-DGUIPP_BUILD_DEPENDENT_LIBS=OFF
-DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF
-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON"

cmake -S. -B ../build/${PWD##*/}/Debug-qt -DCMAKE_BUILD_TYPE=Debug $defines $* 2>&1 | tee -a prepare.log
cmake -S. -B ../build/${PWD##*/}/Release-qt -DCMAKE_BUILD_TYPE=Release $defines $* 2>&1 | tee -a prepare.log

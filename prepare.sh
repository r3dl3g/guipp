echo ======= New Prepare at $(date) ======= >> prepare.log
defines="-DCMAKE_INSTALL_PREFIX=$HOME/install
-DCMAKE_PREFIX_PATH=$HOME/install
-DGUIPP_TESTS=OFF
-DGUIPP_SAMPLES=ON
-DGUIPP_CONFIG_INSTALL=ON
-DGUIPP_BUILD_DEPENDENT_LIBS=OFF
-DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF
-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON"

cmake -S. -B $HOME/build/${PWD##*/}/Debug -DCMAKE_BUILD_TYPE=Debug $defines $* 2>&1 | tee -a prepare.log
cmake -S. -B $HOME/build/${PWD##*/}/Release -DCMAKE_BUILD_TYPE=Release $defines $* 2>&1 | tee -a prepare.log

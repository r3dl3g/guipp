echo ======= New Build at $(date) ======= >> build.log
cmake --build $HOME/build/${PWD##*/}/Debug --parallel 12 --verbose $* 2>&1 | tee -a build.log
cmake --build $HOME/build/${PWD##*/}/Release --parallel 12 $* 2>&1 | tee -a build.log

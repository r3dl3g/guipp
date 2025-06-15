echo ======= New Build at $(date) ======= >> build.log
cmake --build build --parallel 12 $* 2>&1 | tee -a build.log

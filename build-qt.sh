echo ======= New Build at $(date) ======= >> build.log
cmake --build ../build/${PWD##*/}/Debug-qt --parallel 12 --verbose $* 2>&1 | tee -a build.log
cmake --build ../build/${PWD##*/}/Release-qt --parallel 12 $* 2>&1 | tee -a build.log

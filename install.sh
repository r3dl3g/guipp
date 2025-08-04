cmake --build ../build/${PWD##*/}/Debug --target install 2>&1 | tee -a install.log
cmake --build ../build/${PWD##*/}/Release --target install 2>&1 | tee -a install.log

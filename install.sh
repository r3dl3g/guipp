cmake --build $HOME/build/${PWD##*/}/Debug --target install 2>&1 | tee -a install.log
cmake --build $HOME/build/${PWD##*/}/Release --target install 2>&1 | tee -a install.log

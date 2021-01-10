
CXX=x86_64-w64-mingw32-g++
TOOL_CHAIN=/usr/share/mingw-w64/mingw-w64-x86_64.cmake

. ./.generate.sh

generate_all mingw -DCMAKE_TOOLCHAIN_FILE=$TOOL_CHAIN $*

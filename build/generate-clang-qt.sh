
export CC=clang
export CXX=clang++

. ./generate.sh

generate_all clang-qt -DGUIPP_USE_QT=ON $*

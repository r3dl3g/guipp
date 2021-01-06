
. ./generate.sh

generate_all osx -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=/usr/local/opt/qt5 $*

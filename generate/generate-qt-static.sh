
. ./.generate.sh

QTDIR=../qt/static

generate_all qt-static -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=$QTDIR $*

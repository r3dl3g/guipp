
. ./.generate.sh

#QTDIR=/usr/local/opt/qt5
QTDIR=../qt/dist

generate_all osx -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=$QTDIR -DGUIPP_USE_BOOST=ON $*

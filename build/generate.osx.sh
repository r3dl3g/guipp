
builddir=./$(c++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)
prjdir=$PWD/..

build () {
  mkdir -p $builddir/$1
  pushd $builddir/$1
  shift
  type=$1
  shift
  cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="$type" -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=/usr/local/opt/qt5 $*
  popd
}

build release-static Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON
build release-shared Release -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON
build release Release

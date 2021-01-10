
. ./.get_compiler_ver.sh

prjdir=$PWD/..

build () {
  builddir=./$(compiler_ver $*)/$1

  mkdir -p $builddir
  pushd $builddir
  shift
  cmake --build . -- -j $*
  popd
}

build_target_all () {
  prefix=$1
  shift
  target=$1
  shift

  build $prefix-$target-static $*
  build $prefix-$target-shared $*
  build $prefix-$target $*
}

build_all () {
  prefix=$1
  shift

  build_target_all $prefix release $*
  build_target_all $prefix debug $*
}

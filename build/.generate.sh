
. ./.get_compiler_ver.sh

generate () {
  prjdir=$PWD/..

  outdir=$1
  shift
  type=$1
  shift

  builddir=./$(compiler_ver $*)/$outdir

  mkdir -p $builddir
  pushd $builddir
  cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="$type" $*
  popd
}

generate_all () {
  prefix=$1
  shift

  generate $prefix-release-static Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON $*
  generate $prefix-release-shared Release -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON $*
  generate $prefix-release Release $*
  generate $prefix-debug-static Debug -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON $*
  generate $prefix-debug-shared Debug -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON $*
  generate $prefix-debug Debug $*

}

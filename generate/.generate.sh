#!/bin/sh

. ./.outdir.sh

generate () {
  prjdir=$PWD/..

  odir=$1
  shift
  type=$1
  shift

  builddir=$(outdir $odir $*)

  mkdir -p $builddir
  pushd $builddir
  cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="$type" -DGUIPP_BUILD_DEPENDENT_LIBS=ON -DGUIPP_SAMPLES=ON -DGUIPP_TESTS=ON -DGUIPP_TOOLS=ON $*
  popd
}

generate_all () {
  prefix=$1
  shift

  generate $prefix-release-static Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=ON -DUTIL_BUILD_STATIC_MODULE_LIB=ON $*
  generate $prefix-release-shared Release -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=OFF -DUTIL_BUILD_STATIC_MODULE_LIB=OFF $*
  generate $prefix-release Release $*
  generate $prefix-debug-static Debug -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=ON -DUTIL_BUILD_STATIC_MODULE_LIB=ON $*
  generate $prefix-debug-shared Debug -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=OFF -DUTIL_BUILD_STATIC_MODULE_LIB=OFF $*
  generate $prefix-debug Debug $*

}

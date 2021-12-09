#!/bin/sh

. ./.get_compiler_ver.sh

out_base=~/Projekte/build/ibr/guipp

outdir () {
  odir=$1
  shift

  echo $out_base/$(compiler_ver $*)/$odir
}

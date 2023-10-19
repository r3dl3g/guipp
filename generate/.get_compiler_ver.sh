#!/bin/sh

compiler_ver () {
  sys_cxx=def_cxx
  if [ "$CXX" != "" ]; then
    sys_cxx=$CXX
  fi
  sys_info_file=$PWD/.${sys_cxx}_info.tmp
  if [ ! -e $sys_info_file ]; then
    cdir=$PWD
    mkdir -p tmp
    cd tmp
      cmake .. $cmakeFlags $* > $sys_info_file
    cd $cdir
    rm -r ./tmp
  fi
  ver=$(cat $sys_info_file | grep "CMAKE_CXX_COMPILER_VERSION " | head -n1 | awk '{ gsub("\"",""); print $3 }')
  library=$(cat $sys_info_file | grep "COMPILER_BASENAME " | head -n1 | awk '{ gsub("\"",""); print $3 }')
  system=$(cat $sys_info_file | grep "CMAKE_SYSTEM " | head -n1 | awk '{ gsub("\"",""); print $3 }')
  processor=$(cat $sys_info_file | grep "CMAKE_SYSTEM_PROCESSOR " | head -n1 | awk '{ gsub("\"",""); print $3 }')
  echo ${system}-${processor}_${library}-${ver}
}

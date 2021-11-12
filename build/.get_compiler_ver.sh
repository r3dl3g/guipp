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
      cmake .. $* > $sys_info_file
    cd $cdir
    rm -r ./tmp
  fi
  ver=$(cat $sys_info_file | grep "CMAKE_CXX_COMPILER_VERSION" | awk '{ gsub("\"",""); print $3 }')
  platform=$(cat $sys_info_file | grep "CMAKE_CXX_PLATFORM_ID" | awk '{ gsub("\"",""); print $3 }')
  id=$(cat $sys_info_file | grep "CMAKE_CXX_COMPILER_ID" | awk '{ gsub("\"",""); print $3 }')
  echo $platform-$id-$ver

#  if [ "$CXX" == "" ]; then
#    echo $(c++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)
#  else
#    echo $($CXX --version | { read first rest ; echo $first ; })$($CXX -dumpversion)
#  fi
}


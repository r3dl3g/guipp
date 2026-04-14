{ pkgs ? import <nixpkgs> {} }:

let
  # Wir definieren das Cross-System explizit, um isStatic zu forcieren
  targetPkgs = import pkgs.path {
    crossSystem = {
      config = "x86_64-w64-mingw32";
      libc = "msvcrt";
    };
  };
in
targetPkgs.mkShell {
  nativeBuildInputs = with targetPkgs.buildPackages; [
    cmake
    ninja
    binutils
    git
    tig
    mc
  ];

  buildInputs = with targetPkgs; [
    windows.pthreads
    windows.mingw_w64
  ];

  shellHook = ''
    export CC=x86_64-w64-mingw32-gcc
    export CXX=x86_64-w64-mingw32-g++
    export MYPS1='\n\[\033[1;35m\](mingw)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
    export PLATFORM=mingw
  '';
}

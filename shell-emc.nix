{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  hardeningDisable = [ "fortify" ];
  buildInputs = with pkgs; [
    stdenv.cc.cc.lib
  ];

  nativeBuildInputs = with pkgs; [
    cmake
    coreutils
    gnumake
    gdb
    git
    pkg-config
    emscripten
    python3
    wasmtime
    nodejs
  ];

  shellHook = ''
    export MYPS1='\n\[\033[1;35m\](emc)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
    export EM_CACHE=$HOME/.cache/build/emscripten
  '';
}

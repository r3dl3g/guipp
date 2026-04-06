{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  hardeningDisable = [ "fortify" ];

  nativeBuildInputs = with pkgs; [
    cmake
    pkg-config
    coreutils
    gnumake
    gcc
    gdb
    git
    tig
  ];

  buildInputs = with pkgs; [
    SDL2
    SDL2_ttf
    SDL2_gfx
    harfbuzz
  ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=ON"
    "-DGUIPP_USE_SDL=ON"
  ];

  shellHook = ''
    export MYPS1='\n\[\033[1;35m\](SDL)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
    export PLATFORM=sdl
  '';

}

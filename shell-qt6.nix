{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  hardeningDisable = [ "fortify" ];

  nativeBuildInputs = with pkgs; [
    cmake
    coreutils
    gnumake
    gcc
    gdb
    git
    tig
  ];

  buildInputs = with pkgs; [
    kdePackages.qtbase
    kdePackages.qtwayland
    kdePackages.wrapQtAppsHook
  ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=ON"
    "-DGUIPP_USE_QT6=ON"
  ];

  shellHook = ''
    export MYPS1='\n\[\033[1;35m\](Qt6)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
  '';


}

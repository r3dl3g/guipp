{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  hardeningDisable = [ "fortify" ];

  buildInputs = with pkgs; [
    libsForQt5.qt5.wrapQtAppsHook
    libsForQt5.qt5.qtwayland
    libsForQt5.qt5.qtbase
  ];

  nativeBuildInputs = with pkgs; [
    cmake
    coreutils
    gnumake
    gcc
    gdb
    git
    tig
  ];

  shellHook = ''
    export MYPS1='\n\[\033[1;35m\](Qt)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
  '';

}

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
    libsForQt5.qt5.wrapQtAppsHook
    libsForQt5.qt5.qtx11extras
    libsForQt5.qt5.qtbase
  ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=ON"
    "-DGUIPP_USE_QT5=ON"
  ];

  shellHook = ''
    export MYPS1='\n\[\033[1;35m\](Qt5)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
    export QT_PLUGIN_PATH=/nix/store/w0cndkj3cpvqsg6xw3gay90xziqa0zzm-qtbase-5.15.18-bin/lib/qt-5.15.18/plugins
    export QT_QPA_PLATFORM_PLUGIN_PATH=/nix/store/w0cndkj3cpvqsg6xw3gay90xziqa0zzm-qtbase-5.15.18-bin/lib/qt-5.15.18/plugins/platforms
    export QT_QPA_PLATFORM=wayland
  '';

}

{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {

  nativeBuildInputs = with pkgs; [
    cmake
    coreutils
    gnumake
    gcc
    gdb
    git
  ];

  buildInputs = with pkgs; [
    libsForQt515.qt5.wrapQtAppsHook
    libsForQt515.qt5.qtwayland
    libsForQt515.qt5.qtbase
  ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=ON"
    "-DGUIPP_USE_QT5=ON"
    "-DGUIPP_CONFIG_INSTALL=OFF"
    "-DLOGGING_CONFIG_INSTALL=OFF"
    "-DUTIL_CONFIG_INSTALL=OFF"
    "-DTESTING_CONFIG_INSTALL=OFF"
    "-DGUIPP_BUILD_DEPENDENT_LIBS=ON"
    "-DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF"
    "-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON"
    "-DLOGGING_BUILD_STATIC_MODULE_LIB=OFF"
    "-DUTIL_BUILD_STATIC_MODULE_LIB=OFF"
  ];

}

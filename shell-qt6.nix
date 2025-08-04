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
    kdePackages.qtbase
    kdePackages.qtwayland
    kdePackages.wrapQtAppsHook
  ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=ON"
    "-DGUIPP_USE_QT6=ON"
    "-DGUIPP_CONFIG_INSTALL=ON"
    "-DGUIPP_BUILD_DEPENDENT_LIBS=OFF"
    "-DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF"
    "-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON"
  ];

}

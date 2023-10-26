{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    coreutils
    gnumake
    gcc
    gdb
    git
    xcbutilxrm
    xorg.libX11.dev
    xorg.libXft.dev
    xorg.libxcb.dev
    xorg.libXdmcp.dev
  ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=ON"
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

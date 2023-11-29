# https://discourse.nixos.org/t/how-to-use-a-nix-derivation-from-a-local-folder/5498/4

{ stdenv, lib, pkgs } :

stdenv.mkDerivation rec {
  pname = "guipp";
  version = "1.4.0";

  src = ./.;

  nativeBuildInputs = with pkgs; [ cmake ];

  buildInputs = with pkgs; [
    xcbutilxrm
    xorg.libX11.dev
    xorg.libXft.dev
    xorg.libxcb.dev
    xorg.libXdmcp.dev
#    (callPackage ../logging/derivation.nix { })
#    (callPackage ../util/derivation.nix { })
  ];

  enableParallelBuilding = true;

  outputs = [ "out" "dev" ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=OFF"
    "-DGUIPP_CONFIG_INSTALL=ON"
    "-DLOGGING_CONFIG_INSTALL=ON"
    "-DUTIL_CONFIG_INSTALL=ON"
    "-DTESTING_CONFIG_INSTALL=ON"
    "-DGUIPP_BUILD_DEPENDENT_LIBS=ON"
    "-DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF"
    "-DGUIPP_BUILD_SHARED_MODULE_LIBS=OFF"
    "-DLOGGING_BUILD_STATIC_MODULE_LIB=ON"
    "-DUTIL_BUILD_STATIC_MODULE_LIB=ON"
  ];

  meta = with lib; {
    description = "A pure c++ model-view-control based application framework";
    homepage = "https://github.com/r3dl3g/guipp";
    changelog = "https://github.com/r3dl3g/guipp/releases/tag/v${version}";
    license = licenses.mit;
    maintainers = with maintainers; [ r3dl3g ];
    platforms = with platforms; linux;
  };
}

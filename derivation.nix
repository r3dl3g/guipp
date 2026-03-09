# https://discourse.nixos.org/t/how-to-use-a-nix-derivation-from-a-local-folder/5498/4

{ stdenv, lib, pkgs,
  testing ? import ../testing {},
  logging ? import ../logging {},
  util ? import ../util {},
  persistent ? import ../persistent {}
} :

stdenv.mkDerivation rec {
  pname = "guipp";
  version = "1.7.1";

  src = ./.;

  nativeBuildInputs = with pkgs; [ cmake ];

  buildInputs = with pkgs; [
    xcbutilxrm
    xorg.libX11.dev
    xorg.libXft.dev
    xorg.libxcb.dev
    xorg.libXdmcp.dev
    xorg.libXext.dev
    xorg.libXrender.dev
    testing
    util
    logging
    persistent
  ];

  enableParallelBuilding = true;

  outputs = [ "out" "dev" ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_SAMPLES=OFF"
    "-DGUIPP_CONFIG_INSTALL=ON"
    "-DGUIPP_BUILD_DEPENDENT_LIBS=OFF"
    "-DCMAKE_INSTALL_PREFIX=$out"
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

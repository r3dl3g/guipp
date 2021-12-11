with (import <nixpkgs> {});

stdenv.mkDerivation {
  pname = "guipp";
  version = "1.0.1";
  system = builtins.currentSystem;

  src = ./.;

  nativeBuildInputs = [ cmake ];

  buildInputs = [
    xcbutilxrm
    xorg.libX11.dev
    xorg.libXft.dev
    xorg.libxcb.dev
    xorg.libXdmcp.dev
  ];

  enableParallelBuilding = true;

  outputs = [ "out" "dev" ];

  cmakeFlags = [
    "-DGUIPP_TESTS=OFF"
    "-DGUIPP_CONFIG_INSTALL=ON"
  ];

  buildPhase = "cmake --build . -- -j";

  installPhase = "cmake --install .";

  meta = with lib; {
    description = "A pure c++ model-view-conrol based application framework";
    homepage = "https://github.com/r3dl3g/guipp";
    changelog = "https://github.com/r3dl3g/guipp/releases/tag/v${version}";
    license = licenses.mit;
    maintainers = with maintainers; [ r3dl3g ];
    platforms = with platforms; linux;
  };
}

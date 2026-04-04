{ pkgs ? import <nixpkgs> {} }:

let
  # Wir nutzen weiterhin das mingwW64-Set für die Windows-Header/Libs
  targetPkgs = pkgs.pkgsCross.mingwW64;
in
targetPkgs.mkShell {
  nativeBuildInputs = with targetPkgs.buildPackages; [
    cmake
    ninja
    # Wir erzwingen die Nutzung von Clang aus dem Cross-Set
    llvmPackages.clang
    llvmPackages.llvm
    git
    tig
  ];

  buildInputs = with targetPkgs; [
    # Hier kannst du weitere Windows-Abhängigkeiten hinzufügen
    windows.pthreads
  ];

  # Hier setzen wir die MSVC-Kompatibilitäts-Flags für Clang
  # -fms-compatibility: Emuliert MSVC-spezifisches Verhalten
  # -fms-extensions: Erlaubt Microsoft-spezifische Sprach-Erweiterungen
  # -fmsc-version: Simuliert eine bestimmte MSVC-Version (1930 = VS 2022)
  NIX_CFLAGS_COMPILE = [
    "-fms-compatibility"
    "-fms-extensions"
    "-fmsc-version=1930"
    "-Wno-microsoft-include" # Unterdrückt Warnungen bei Pfad-Differenzen (Case-Sensitivity)
  ];

  # Statisches Linking wie zuvor gewünscht
  NIX_LDFLAGS = "-static";

  shellHook = ''
    # Wir biegen die Standard-Compiler-Variablen explizit auf Clang um
    export CC=x86_64-w64-mingw32-clang
    export CXX=x86_64-w64-mingw32-clang++
    export MYPS1='\n\[\033[1;35m\](clang-w32)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
    export PLATFORM=win32
    
    echo "Clang-MSVC Compatibility Environment geladen."
    echo "Target: x86_64-w64-mingw32 (static)"
    echo "Compiler: $CXX mit -fms-compatibility"
  '';
}

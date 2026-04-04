{ pkgs ? import <nixpkgs> {} }:

let
  # Wir definieren das Cross-System explizit, um isStatic zu forcieren
  targetPkgs = import pkgs.path {
    crossSystem = {
      config = "x86_64-w64-mingw32";
      libc = "msvcrt";
      isStatic = true; # Forciert statische Builds für alle Pakete in diesem Set
    };
  };
in
targetPkgs.mkShell {
  nativeBuildInputs = with targetPkgs.buildPackages; [
    cmake
    ninja
    binutils # Enthält Tools wie 'strip' für Windows Executables
    git
    tig
  ];

  buildInputs = with targetPkgs; [
    # Falls du weitere Libs brauchst, z.B. openssl oder zlib,
    # würden diese hier durch 'isStatic = true' automatisch als .a statt .dll geladen.
    windows.pthreads # Häufige Abhängigkeit für C++11 Threads unter MinGW
  ];

  # Das ist der entscheidende Teil für deine Linker-Flags
  NIX_CFLAGS_COMPILE = "-static-libgcc -static-libstdc++";
  NIX_LDFLAGS = "-static";

  shellHook = ''
    echo "Statisches Windows-Build-Environment bereit."
    export CC=x86_64-w64-mingw32-gcc
    export CXX=x86_64-w64-mingw32-g++
    export MYPS1='\n\[\033[1;35m\](mingw)[\[\e]0;\u@\h: \w\a\]\u@\h:\w]\$\[\033[0m\] '
    export PS1=$MYPS1
    export PLATFORM=win32
  '';
}

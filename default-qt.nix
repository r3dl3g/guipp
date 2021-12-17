{ pkgs ? import <nixpkgs> {} }:

pkgs.callPackage ./derivation-qt.nix {}

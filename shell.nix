{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "uv-c-build-env";

  # packages replaces nativeBuildInputs in modern Nix configurations
  packages = with pkgs; [
    # Core compiler toolchain wrappers (automatically injects glibc headers/libs)
    stdenv.cc
    gnumake
    pkg-config

    # Expose standard development headers
    glibc.dev
  ];

  shellHook = ''
    echo "=== C Compilation Shell Active ==="
    echo "GCC Path: $(which gcc)"
  '';
}

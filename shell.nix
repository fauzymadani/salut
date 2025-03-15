{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  buildInputs = [
    pkgs.fmt
    pkgs.clang
    pkgs.clang-tools
  ];
  shellHook = ''
    export LD_LIBRARY_PATH=${pkgs.stdenv.cc.cc.lib}/lib:${pkgs.libz}/lib:$LD_LIBRARY_PATH
    echo -e "\e[34mBuilding salut\e[0m"
    clang++ -o salut main.cpp -lfmt
    echo -e "\e[32mFinished building salut\e[0m"
    exit
  '';
}

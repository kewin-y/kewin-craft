{
  description = "Minecraft Clone";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = {nixpkgs, ...}: let
    forAllSystems = nixpkgs.lib.genAttrs [
      "aarch64-linux"
      "x86_64-linux"
      "x86_64-darwin"
      "aarch64-darwin"
    ];
  in {
    formatter = forAllSystems (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
      in
        pkgs.alejandra
    );
    devShells = forAllSystems (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        default = pkgs.mkShell {
          buildInputs = builtins.attrValues {
            inherit
              (pkgs)
              valgrind-light
              clang-tools
              cmake
              pkg-config
              wayland
              libffi
              libxkbcommon
              wayland-scanner
              libx11
              libxrandr
              libxi
              libxinerama
              libxcursor
              gdb
              ;
          };

          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (builtins.attrValues {
            inherit
              (pkgs)
              wayland
              libxkbcommon
              libGL
              libglvnd
              mesa
              ;
          });
        };
      }
    );
  };
}

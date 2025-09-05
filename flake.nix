{
  description = "Can't wait to";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs @ {flake-parts, ...}:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"];
      perSystem = {
        config,
        self',
        inputs',
        pkgs,
        system,
        ...
      }: {
        devShells.default = pkgs.mkShell {
          buildInputs = [
            pkgs.bear
            pkgs.valgrind-light
            pkgs.clang-tools
            pkgs.cmake
            pkgs.pkg-config

            pkgs.xorg.libX11
            pkgs.xorg.libXrandr
            pkgs.xorg.libXi
            pkgs.xorg.libXinerama
            pkgs.xorg.libXcursor
            pkgs.wayland
            pkgs.libffi
            pkgs.libxkbcommon
            pkgs.wayland-scanner
          ];

          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
            # pkgs.xorg.libX11
            # pkgs.xorg.libXrandr
            # pkgs.xorg.libXi
            # pkgs.xorg.libXinerama
            # pkgs.xorg.libXcursor
            # TODO: Figure out X11 libs (will the compiled binary even run on X11?)
            pkgs.wayland
            pkgs.libxkbcommon
            pkgs.libGL
            pkgs.libglvnd
            pkgs.mesa
          ];

          shellHook = ''
            export SHELL='${pkgs.mksh}/bin/mksh'
          '';
        };
      };
    };
}

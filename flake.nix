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
          packages = with pkgs; [
            bear
            valgrind-light
            clang-tools
          ];

          buildInputs = with pkgs; [
            glfw
            glm
            stb
            xorg.libX11
            xorg.libXrandr
            xorg.libXi
          ];
        };
      };
    };
}

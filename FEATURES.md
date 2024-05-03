### Build changes

## HiGHS on nixpkgs

HiGHS now has a `flake.nix` to build the binary, allowing `nix` users to try it out

## Python build update

Highspy with setuptools from v1.7.0 only worked on Python 3.12
For v1.7.0 we have dropped setuptools and switched to scikit-build-core

## Windows versions

Fixed version info of shared library
Added version info to executable

### Code changes

Inserting `pdlp_iteration_count` into various structs (for v1.7.0) breaks the C API, so it has been moved to the end of those structs


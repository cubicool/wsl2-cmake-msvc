# Overview

This project is small collection of scripts, configuration templates and
documentation for simplifying the process of using CMake within WSL2 with the
[MSVC Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019).

## Build/Setup

Here we demonstrate setting up a simple C++ project inside a WSL2 terminal. If
you haven't discovered them yet, [Alacritty](https://github.com/alacritty/alacritty)
and [Windows Terminal](https://docs.microsoft.com/en-us/windows/terminal/) are
both great alternatives to the default **cmd.exe** or even **PowerShell**.

> **NOTE** Your source directory (indicated below as *MY_PROJECT*) **MUST**
> physically be on/within an NTFS filesystem, and not a directory that lives
> solely inside your WSL2 image! **This is essential!**

```sh
cd MY_PROJECT

mkdir build-wsl
cd build-wsl

# Call our custom script with the parent directory as the root of the
# source tree.
cmake-wsl ..

# The "cmake-wsl" script will have created a convenient Makefile acting
# as wrapper for kicking off builds WITHOUT having to open up an IDE or
# use msbuild.
make
```

## Tests

There are a few different C sources included in the
[test](https://github.com/cubicool/wsl2-cmake-msvc/tree/main/test) directory.
These can be compiled using the recipe above. Make sure you follow the source
directory **NOTE** above!

## TODO

- Better [policy](https://cmake.org/cmake/help/latest/manual/cmake-policies.7.html) support

- Make creation of **install** and **uninstall** targets easier.

- [Modern CMake Guide](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1)

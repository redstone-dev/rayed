# `rayed`
Code editor written in C++ with raylib

<small>fun fact: this README was written with it!</small>

<hr />

## Compiling

Because I'm stupid, you need to install raylib as a system package, which
is very easy on linux -- just use your package manager, and search for
`raylib`, `raylib-dev` or `raylib-devel`.

I don't know how to do that on Windows. On macOS, brew might work.

After installing raylib, just run `build.sh`. 

> To create a tarball for
a github release, run `build.sh -A<arch> -O<os>`, where `<arch>` is your
CPU architecture (`amd64`/`aarch64`/whatever) and `<os>` is your OS
(`linux`/`mac`/`win`).

## Bugs & crashes

Find a bug or a crash? Please report it! 

Specifically for segfaults, you should ideally:
  1. compile `rayed` from source with debug flags
  2. pop the resulting executable into `gdb` and run it
  3. replicate said segfault
  4. get a backtrace (enter `bt` into the `gdb` console) 
  5. include said backtrace in your report

# `rayed`
Code editor written in C++ with raylib

<small>fun fact: this README was written with it!</small>

<hr />

## Compiling

To build the project, you need to install raylib as a system package, which
is very easy on Linux -- just use your package manager, and search for
`raylib`, `raylib-dev` or `raylib-devel`. Also install Lua in a similar vein.

I don't know how to do that on Windows. On macOS, brew might work.

After installing raylib, you can build the project using the provided Makefile:

```bash
make
```

To run the program after building:

```bash
make run
```

To build the program with debug flags:

```bash
make debug
```

To clean up the build directory completely:

```bash
make clean
```

> To create a tarball for a GitHub release, use the `release` target:
> 
> ```bash
> make release ARCH=<arch> OS=<os>
> ```
> 
> Replace `<arch>` with your CPU architecture (`amd64`/`aarch64`/whatever) and `<os>` with your OS (`linux`/`mac`/`win`).

## Bugs & crashes

Find a bug or a crash? Please report it! 

Specifically for segfaults, you can use the `make debug` target, which:
  1. compiles `rayed` with debug flags
  2. launches the program in `gdb`
  3. automatically generates a backtrace and saves it to `build/backtrace.txt`

Include the contents of `backtrace.txt` in your report for easier debugging.

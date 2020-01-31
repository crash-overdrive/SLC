# SLC

SLC is a compiler. Tell our story here...

### Development

The project uses CMake as a build tool. To build the `Makefile` out-of-source, run

```sh
$ mkdir build
$ cd build
$ cmake ..
```

Then, to build the `joosc` executable, run

```sh
$ make
```

The executable will be under the `bin` directory.

To run tests,

```sh
$ ctest
```
To run tests with Valgrind, add `-T memcheck` option.

To submit the project to Marmoset,

```sh
make submit
```

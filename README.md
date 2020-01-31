# SLC

SLC is a compiler. Tell our story here...

### Development

The project uses CMake as a build tool. To generate a Makefile for your platform, simply run

```sh
$ cmake .
```

Then, to build the `joosc` executable, run

```sh
$ make
```

To run tests with Valgrind,

```sh
$ ctest -T memcheck
```

To submit the project to Marmoset,

```sh
make submit
```
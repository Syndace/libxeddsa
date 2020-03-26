[![Documentation Status](https://readthedocs.org/projects/libxeddsa/badge/?version=latest)](https://libxeddsa.readthedocs.io/en/latest/?badge=latest)
[![Build Status](https://travis-ci.org/Syndace/libxeddsa.svg?branch=master)](https://travis-ci.org/Syndace/libxeddsa)

# libxeddsa #

An implementation of [XEdDSA](https://www.signal.org/docs/specifications/xeddsa/), based on ref10 by Daniel J. Bernstein.

XEdDSA is a signature scheme that utilizes the birational maps between Curve25519 and Ed25519 (defined in [RFC 7748](https://tools.ietf.org/html/rfc7748) on page 5) to create and verify digital signatures with Curve25519 keys.

XEdDSA is also specified for Curve448/Ed448, which is not covered by this library.

## Installation ##

Prebuilt binaries (x86/amd64) are available for Linux, MacOS and Windows and attached to [the releases](https://github.com/Syndace/libxeddsa/releases). The dynamic variants (`.so`, `.dylib` and `.dll`) require [libsodium](https://download.libsodium.org/doc/) to be installed on the system.

The library can be built with [CMake](https://cmake.org/) and was tested to build successfully on Linux, MacOS and Windows.

### Linux ###

To build the library from source on Linux, `cmake` and `make` are required next to a C compiler and libsodium + development headers. On apt-based systems (Debian, Ubuntu, Mint) these are usually available in packages called `cmake`, `build-essential`, `libsodium-dev` and `libsodiumxx`, where `xx` is a two-digit number that differs between systems and versions (e.g. 23 on Debian Buster and 18 on Ubuntu 16.04).

With everything installed, run the following steps to build the library and to run the tests:

```Bash
$ mkdir build/
$ cd build/
$ cmake ..
$ make
$ ctest ..
```

The static and dynamic libraries will be located in `bin/` after the build, together with the test executables.

### MacOS ###

To build the library from source on MacOS, `cmake` and `make` are required next to a C compiler and libsodium. CMake, make and libsodium can be installed from [Homebrew](https://brew.sh/) and are available in packages called `cmake`, `make` and `libsodium`.

With everything installed, run the following steps to build the library and to run the tests:

```Bash
$ mkdir build/
$ cd build/
$ cmake ..
$ make
$ ctest ..
```

The static and dynamic libraries will be located in `bin/` after the build, together with the test executables.

### Windows ###

WIP

## Bindings to other Programming Languages ##

| Language | Bindings | Notes |
|----------|----------|-------|
| Python   | [python-xeddsa](https://github.com/Syndace/python-xeddsa) | Provides an object-oriented interface next to raw ctype bindings. |

## A Note on Security ##

This library tries to provide a secure implementation by

- using only constant-time primitives with secret data,
- not conditionally branching on secret data,
- overwriting stack variables with zeros before returning to not leave any secret data on the stack.

Please note that while these precautions are generally a good idea to make side-channel attacks harder, the code was not written by a cryptographer and was not audited either (yet).

## A Note on Tests ##

Sadly the XEdDSA specification does not provide test vectors. Thus, there are currently just two types of tests:

- Do signatures created by this library validate with libsodium's Ed25519 implementation?
- Is the conversion of public keys from Curve25519 to Ed25519 and back to Curve25519 unique?

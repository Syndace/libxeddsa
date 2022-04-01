Installation
============

Prebuilt binaries (for amd64) are available for Linux, MacOS and Windows and attached to `the releases <https://github.com/Syndace/libxeddsa/releases>`_. The dynamic variants (``.so``, ``.dylib`` and ``.dll``) require `libsodium <https://download.libsodium.org/doc/>`_ to be installed on the system.

The library can be built with `CMake <https://cmake.org/>`_ and was tested to build successfully on Linux, MacOS and Windows.

Linux
-----

To build the library from source on Linux, ``cmake`` and ``make`` are required next to a C compiler and libsodium + development headers. On apt-based systems (Debian, Ubuntu, Mint) these are usually available in packages called ``cmake``, ``build-essential``, ``libsodium-dev`` and ``libsodiumxx``, where ``xx`` is a two-digit number that differs between systems and versions (e.g. 23 on Debian Bullseye and 18 on Ubuntu 16.04).

With everything installed, run the following steps to build the library and to run the tests::

    $ mkdir build/
    $ cd build/
    $ cmake ..
    $ make
    $ ctest ..

The static and dynamic libraries will be located in ``bin/`` after the build, together with the test executables.

MacOS
-----

To build the library from source on MacOS, ``cmake`` and ``make`` are required next to a C compiler and libsodium. CMake, make and libsodium can be installed from `Homebrew <https://brew.sh/>`_ and are available in packages called ``cmake``, ``make`` and ``libsodium``.

With everything installed, run the following steps to build the library and to run the tests::

    $ mkdir build/
    $ cd build/
    $ cmake ..
    $ make
    $ ctest ..

The static and dynamic libraries will be located in ``bin/`` after the build, together with the test executables.

Windows
-------

To build the library from source on Windows, ``cmake``, libsodium and a build environment like MinGW, MSYS or Visual Studio are required. CMake can be installed using the `binary installer package <https://cmake.org/download/>`_ from CMake's website. libsodium headers and binaries are available on `libsodium's download page <https://download.libsodium.org/libsodium/releases/>`_. The exact build process depends on you build environment. In general, the first step is to use CMake to create the build files for you build environment::

    $ mkdir build/
    $ cd build/
    $ cmake -G "YourBuildEnv" ..

where ``YourBuildEnv`` depends on your build environment. ``cmake --help`` lists available build systems you can pass to ``-G``. If CMake has trouble locating libsodium, set the ``sodium_DIR`` environment variable to the directory containing the headers and the files.

The next step is to build the library, which again depends on your build system. With MinGW for example, use ``mingw32-make``.

When the build is done, run ``ctest ..`` to run the tests.

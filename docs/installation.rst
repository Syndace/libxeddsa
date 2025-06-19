Installation
============

Prebuilt binaries are available for Linux (amd64 & arm64), MacOS (amd64 & arm64) and Windows (amd64) and attached to `the releases <https://github.com/Syndace/libxeddsa/releases>`_. They can also be found in the build artifacts of the `Build & Test workflow <https://github.com/Syndace/libxeddsa/actions/workflows/build-and-test.yml>`_. To use libxeddsa, `libsodium <https://download.libsodium.org/doc/>`_ has to be installed on the system.

The library can be built with `CMake <https://cmake.org/>`_ and was tested to build successfully on Linux, MacOS and Windows.

Linux
-----

To build the library from source on Linux, ``cmake`` and ``make`` are required next to a C compiler and libsodium + development headers. On apt-based systems (Debian, Ubuntu, Mint) these are usually available in packages called ``cmake``, ``build-essential``, ``libsodium-dev`` and ``libsodiumxx``, where ``xx`` is a two-digit number that differs between systems and versions (e.g. 23 on the latest Debian and Ubuntu).

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

When the build is done, run ``ctest -C Debug ..`` to run the tests in debug configuration, and ``ctest -C Release ..`` to run them in release configuration.

Emscripten
----------

The library can be compiled for the web using `Emscripten <https://emscripten.org/>`_. To so so, `set up emsdk <https://emscripten.org/docs/getting_started/downloads.html>`_ and proceed as follows (instructions for Linux):

Download and build libsodium with Emscripten
""""""""""""""""""""""""""""""""""""""""""""

libsodium supports compilation with Emscripten and comes with the scripts required to do so::

    $ mkdir /tmp/libxeddsa-emscripten/
    $ cd /tmp/libxeddsa-emscripten/
    $ wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.20-stable.tar.gz
    $ tar xzf libsodium-1.0.20-stable.tar.gz
    $ cd libsodium-stable/
    $ dist-build/emscripten.sh --standard

Download and build libxeddsa with Emscripten
""""""""""""""""""""""""""""""""""""""""""""

libxeddsa uses CMake, which is supported by Emscripten::

    $ cd /tmp/libxeddsa-emscripten/
    $ git clone https://github.com/Syndace/libxeddsa.git
    $ cd libxeddsa/
    $ mkdir build/
    $ cd build/
    $ export sodium_INCLUDE_DIR=/tmp/libxeddsa-emscripten/libsodium-stable/libsodium-js/include
    $ export sodium_LIBRARY=/tmp/libxeddsa-emscripten/libsodium-stable/libsodium-js/lib/libsodium.a
    $ emcmake cmake ..
    $ emmake make
    $ emcmake ctest ..

Link the WebAssembly module
"""""""""""""""""""""""""""

With libsodium and libxeddsa compiled for Emscripten, the static libraries can be linked together and the relevant symbols exported::

    $ cd /tmp/libxeddsa-emscripten/libxeddsa/bin/
    $ emcc --no-entry -o libxeddsa.js -O3 \
        -I ../include/ -I ../ref10/include/ -I $sodium_INCLUDE_DIR \
        -L . -L $(dirname $sodium_LIBRARY) -l sodium -l combined_static \
        -s EXPORTED_FUNCTIONS=$(cat ../emscripten/exported_functions.txt | tr -d "\n") \
        -s EXPORTED_RUNTIME_METHODS=ccall,cwrap

This creates the two files ``libxeddsa.js`` and ``libxeddsa.wasm``, which can now be embedded in a JavaScript application. The file ``emscripten/libxeddsa-wrapper.js`` can be loaded after the module is fully intialized (see `Emscripten's "Module.onRuntimeInitialized" <https://emscripten.org/docs/api_reference/module.html#Module.onRuntimeInitialized>`_) to create simple wrappers of the exported functions for direct use from JavaScript.

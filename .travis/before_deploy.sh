cd ${TRAVIS_BUILD_DIR}/bin/

if [ "$TRAVIS_OS_NAME" = "linux" ]
then
    mv libcombined_static.a   libxeddsa-linux-amd64.a
    mv libcombined_dynamic.so libxeddsa-linux-amd64.so
fi

if [ "$TRAVIS_OS_NAME" = "osx" ]
then
    mv libcombined_static.a      libxeddsa-macos-amd64.a
    mv libcombined_dynamic.dylib libxeddsa-macos-amd64.dylib
fi

if [ "$TRAVIS_OS_NAME" = "windows" ]
then
    mv combined_static.lib  libxeddsa-windows-amd64.lib
    mv combined_dynamic.lib libxeddsa-windows-amd64.dll.lib
    mv combined_dynamic.dll libxeddsa-windows-amd64.dll
fi

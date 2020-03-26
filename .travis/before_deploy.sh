cd ${TRAVIS_BUILD_DIR}/bin/

if [ "$TRAVIS_OS_NAME" = "osx" ]
then
    mv libxeddsa_static.a      libxeddsa-macos-amd64.a
    mv libxeddsa_dynamic.dylib libxeddsa-macos-amd64.dylib
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]
then
    mv libxeddsa_static.a   libxeddsa-linux-amd64.a
    mv libxeddsa_dynamic.so libxeddsa-linux-amd64.so
fi

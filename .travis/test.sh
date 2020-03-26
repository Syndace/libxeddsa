cd ${TRAVIS_BUILD_DIR}/build/

if [ "$TRAVIS_OS_NAME" = "linux" ]
then
    ctest ..
fi

if [ "$TRAVIS_OS_NAME" = "osx" ]
then
    ctest ..
fi

if [ "$TRAVIS_OS_NAME" = "windows" ]
then
    cp ../ref10/bin/*.dll ../bin/

    mv ../bin/libsodium_debug.dll   ../bin/libsodium.dll
    ctest -C Debug ..   || true

    mv ../bin/libsodium_release.dll ../bin/libsodium.dll
    ctest -C Release .. || true
fi

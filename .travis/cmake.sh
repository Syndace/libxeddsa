cd ${TRAVIS_BUILD_DIR}
mkdir build/
cd build/

if [ "$TRAVIS_OS_NAME" = "linux" ]
then
    FORCE_URANDOM= cmake ..
fi

if [ "$TRAVIS_OS_NAME" = "osx" ]
then
    FORCE_URANDOM= cmake ..
fi

if [ "$TRAVIS_OS_NAME" = "windows" ]
then
    cmake -T host=x64 -A x64 ..
fi

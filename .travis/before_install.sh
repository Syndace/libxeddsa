cd ${TRAVIS_BUILD_DIR}

if [ "$TRAVIS_OS_NAME" = "windows" ]
then
    wget "https://download.libsodium.org/libsodium/releases/libsodium-1.0.18-stable-msvc.zip" >/dev/null
    unzip "libsodium-1.0.18-stable-msvc.zip" >/dev/null
    export sodium_DIR="$(pwd)/libsodium/"
fi

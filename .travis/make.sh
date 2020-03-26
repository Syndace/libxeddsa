cd ${TRAVIS_BUILD_DIR}/build/

if [ "$TRAVIS_OS_NAME" = "osx" ]
then
    make
fi

if [ "$TRAVIS_OS_NAME" = "linux" ]
then
    make
fi

if [ "$TRAVIS_OS_NAME" = "windows" ]
then
    export MSBUILD_PATH="C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\MSBuild\\15.0\\Bin"
    export PATH=$MSBUILD_PATH:$PATH
    MSBuild.exe libxeddsa.sln
fi


echo ON

if "%platform%"=="Win32" (
    set generator="Visual Studio 15"
    set qt_arch=msvc2015
) else (
    set generator="Visual Studio 15 Win64"
    set qt_arch=msvc2017_64
)

set USE_QT_VER=6.0
set PATH=C:\Qt\%USE_QT_VER%\%qt_arch%\bin;C:\Program Files\CMake\bin;%PATH%
set CMAKE_PREFIX_PATH=C:/Qt/%USE_QT_VER%/%qt_arch%;c:/projects/install
set GTEST_PATH=c:\projects\googletest

if EXIST c:/projects/install goto :gtest

rem setup gmock and gtest
:gtest
if EXIST c:/projects/googletest goto :open_library
git clone https://github.com/google/googletest.git %GTEST_PATH%

rem open library
:open_library
mkdir build
cd build
cmake -G%generator% -DGTEST_DIR=%GTEST_PATH%/googletest -DGMOCK_DIR=%GTEST_PATH%/googlemock -DCMAKE_INSTALL_PREFIX=c:/projects/install/ ..

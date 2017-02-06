set SHRUN_ROOT_PATH=%~dp0

set INSTALL_DIR=%SHRUN_ROOT_PATH%\distrib.install

set QMAKESPEC=win32-msvc2015
set QTDIR=c:\Libraries\Qt-5.7.1-vs2015-x64-official\5.7\msvc2015_64
set QTVER=571

set CMAKE_PATH=d:\devel\CMake-3.5\bin
set DOXYGEN=C:\Program Files (x86)\doxygen\bin
set LATEX_PATH=C:\MiKTeX\miktex\bin

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64

if exist set_user_paths.cmd call set_user_paths.cmd

set PATH=%CMAKE_PATH%;%QTDIR%;%DOXYGEN%;%LATEX_PATH%;%PATH%

mkdir build-VisualStudio
cd build-VisualStudio
cmake -Wno-dev -G "Visual Studio 14 2015 Win64" -DCMAKE_BINARY_DIR:PATH="." -DCMAKE_INSTALL_PREFIX:PATH="%INSTALL_DIR%" -DCMAKE_CONFIGURATION_TYPES:STRING="Debug;Release" -DUSE_FFMPEG:STRING="OFF" ..
cd ..
rem call set_build_number.cmd

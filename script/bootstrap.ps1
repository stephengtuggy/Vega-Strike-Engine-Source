# @file   : bootstrap.ps1
# @brief  : installs dependencies for building Vega Strike on Windows, using vcpkg
# @usage  : .\bootstrap.ps1
# @param  : none

Push-Location ..

git clone https://github.com/microsoft/vcpkg.git
Set-Location vcpkg

.\bootstrap-vcpkg.bat -disableMetrics

.\vcpkg.exe integrate install

.\vcpkg.exe install boost
.\vcpkg.exe install expat
.\vcpkg.exe install freeglut
.\vcpkg.exe install gtk
.\vcpkg.exe install libpng
.\vcpkg.exe install libjpeg-turbo
.\vcpkg.exe install libvorbis
.\vcpkg.exe install openal-soft
.\vcpkg.exe install opengl
.\vcpkg.exe install opengl-registry
.\vcpkg.exe install sdl1
.\vcpkg.exe install zlib

Pop-Location

mkdir build
Set-Location build

..\..\vcpkg\downloads\tools\cmake-3.17.2-windows\cmake-3.17.2-win32-x86\bin\cmake-gui.exe -DUSE_PYTHON_3=ON ..\engine

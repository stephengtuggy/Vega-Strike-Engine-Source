# @file   : build.ps1
# @brief  : Starts CMake and build process
# @usage  : .\script\build.ps1
# @param  : None

mkdir build
Set-Location build

..\..\vcpkg\downloads\tools\cmake-3.17.2-windows\cmake-3.17.2-win32-x86\bin\cmake-gui.exe -DUSE_PYTHON_3=ON ..\engine

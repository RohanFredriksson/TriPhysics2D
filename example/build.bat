@echo off
git submodule update --init --recursive
echo Y | rmdir dependencies\trip2d /s > nul
mkdir dependencies\trip2d
echo d | xcopy ..\src dependencies\trip2d\src /E > nul
echo d | xcopy ..\include dependencies\trip2d\include /E > nul
echo d | xcopy ..\dependencies dependencies\trip2d\dependencies /E > nul
echo d | xcopy ..\CMakeLists.txt dependencies\trip2d\ > nul
echo d | xcopy ..\trip2d.hpp dependencies\trip2d\ > nul
if not exist "build" mkdir build
cd build
if exist posix (
    del CMakeCache.txt /s /f /q > nul
) else (
    if not exist windows (
        del CMakeCache.txt /s /f /q > nul
    )
)
del posix /s /f /q > nul
type nul > windows
cmake .. -G "MinGW Makefiles"
mingw32-make
echo Y | rmdir assets /s > nul
echo d | xcopy ..\assets assets /E > nul

#!/bin/bash
git submodule update --init --recursive
rm -rf dependencies/trip2d
mkdir dependencies/trip2d
cp -r ../src dependencies/trip2d/
cp -r ../include dependencies/trip2d/
cp -r ../dependencies dependencies/trip2d/dependencies
cp ../CMakeLists.txt dependencies/trip2d/
mkdir build 2> /dev/null
cd build
if [ -f windows ]; then
    rm CMakeCache.txt
elif [ ! -f posix ]; then
    rm CMakeCache.txt
fi
rm windows 2> /dev/null
touch posix
cmake ..
make
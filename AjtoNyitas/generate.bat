
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
mkdir build
cd build
set SDL3_DIR=C:\SDL
cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=nmake -DCMAKE_SYSTEM_VERSION=10.0.26100.0
nmake
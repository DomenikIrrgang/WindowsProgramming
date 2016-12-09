@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall" x64
mkdir ..\build
pushd ..\build
cl ..\code\BloomEngine.cpp
popd
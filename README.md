# SilverBin Game Engine
SilverBin is a game engine that seeks to provide a Roblox-like development environment, but with a lot more freedom over user hardware.
Please note that this is still in early development and progress will most likely not be made for a long time.

## Features
- OpenGL Rendering
- Basic camera system
- Texture loading
**More features will be added as it's worked on**
## Prerequisites
- MinGW64
- CMake
## Build Instructions
```
git clone https://github.com/starryband/silverbin.git
cd silverbin

mkdir build
cd build

cmake -B build
cmake --build build
```
## Running
1. Follow the previous instructions in Build Instructions
2. Run the following commands:
```
cd bin
./silver.exe
```
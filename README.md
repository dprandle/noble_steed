# Building

## Linux

### Install git, cmake, build tools, and clang (optional)

sudo apt install git cmake build-essential clang

### Clone repo and init/update submodules

git clone https://github.com/dprandle/Noble_Steed.git
cd Noble_Stted && git submodule init && git submodule update

### Install dependencies
sudo apt-get install libgl1-mesa-dev x11proto-core-dev libx11-dev python3 python3-dev xorg-dev

### Build bgfx

cd deps/bgfx
make <linux/osx/vs2017>-<debug/release><32/64>
cd ..

where you choose one option within the carrots - ie for linux could be linux-release64.

### Build rttr
cd rttr

The cmake config parameters to use are as follows:

"BUILD_DOCUMENTATION":false,
"BUILD_EXAMPLES":false,
"BUILD_INSTALLER":false,
"BUILD_PACKAGE":false,
"BUILD_UNIT_TESTS":false

mkdir build && cd build && cmake -DBUILD_UNIT_TESTS=false *etc etc from above* ..
make -j
cd ../../..

### Build Noble_Steed

The cmake config parameters to use are as follows:

"ASSIMP_BUILD_TESTS":false,
"BUILD_BULLET2_DEMOS":false,
"BUILD_CPU_DEMOS":false,
"BUILD_OPENGL3_DEMOS":false,
"GLFW_BUILD_DOCS":false,
"GLFW_INSTALL":false,
"BUILD_EXTRAS":false,
"BUILD_UNIT_TESTS":false,
"SPDLOG_BUILD_SHARED":true

mkdir build && cd build && cmake -DBUILD_UNIT_TESTS=false *etc etc from above* ..
make -j
cd ..

This should have built the project in Linux

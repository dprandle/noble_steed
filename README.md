# Building

## Linux

### Install git, cmake, build tools, and clang (optional)

```sh
sudo apt install git cmake build-essential clang
```

### Clone repo and init/update submodules

```sh
git clone https://github.com/dprandle/Noble_Steed.git
cd Noble_Steed
git submodule init
git submodule update
```

### Install dependencies

```sh
sudo apt-get install libgl1-mesa-dev x11proto-core-dev libx11-dev python3 python3-dev xorg-dev
```

### Build bgfx

```sh
cd deps/bgfx
make <linux/osx/vs2017>-<debug/release><32/64> -j
cd ..
```

where you choose one option within the carrots - ie for linux could be linux-release64.

### Build Noble_Steed

The cmake config parameters to use are as follows:

```js
"ASSIMP_BUILD_TESTS":false,
"BUILD_BULLET2_DEMOS":false,
"BUILD_CPU_DEMOS":false,
"BUILD_OPENGL3_DEMOS":false,
"GLFW_BUILD_DOCS":false,
"GLFW_INSTALL":false,
"BUILD_EXTRAS":false,
"BUILD_UNIT_TESTS":false,
"SPDLOG_BUILD_SHARED":true
```

```sh
mkdir build && cd build
cmake -DBUILD_UNIT_TESTS=false *etc etc from above* ..
make -j
cd ..
```

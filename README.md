# Requirements for build and run:

## Linux:
```bash
sudo apt update
sudo apt install xorg-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libglfw3-dev libglm-dev
```

## Windows:
```powershell
vcpkg install glfw3 glm
```

# Compiling from Linux:

## Linux:

### Requirements:
```bash
sudo apt update
sudo apt install xorg-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libglfw3-dev libglm-dev
```

### Build:
```bash
rm -rf build/
cmake -S ./ -B build/
cmake --build build/
```

## Windows:

### Requirements:
```bash
sudo apt update
sudo apt install mingw-w64
adding all DLL files. AFTER BUILD!
```

### Build:
```bash
rm -rf build-windows/
cmake -S . -B build-win \ -DCMAKE_SYSTEM_NAME=Windows \ -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \ -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
cmake --build build-win
```
### search DLL. AFTER BUILD!!!:
```bash
x86_64-w64-mingw32-objdump -p build-windows/src/OpenGL.exe | grep "DLL Name"
```


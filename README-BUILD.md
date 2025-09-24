# Build Instructions

## Prerequisites
- CMake 3.10+
- For Linux: g++ with C++17 support
- For Windows cross-compilation: MinGW-w64 (`sudo apt-get install mingw-w64`)

## Building for Linux

```bash
mkdir -p build-linux
cd build-linux
cmake ..
make
cd ..
./build-linux/gambling-calc
```

## Cross-compiling for Windows

```bash
mkdir -p build-windows
cd build-windows
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows.cmake ..
make
```

The Windows executable will be `build-windows/gambling-calc.exe`

## Configuration Files

Both builds use the same `config/` directory containing:
- `federal_rules.cfg` - Federal tax rules
- `state_rules.cfg` - All 50 state tax rules

Edit these files to update tax rules without recompiling!
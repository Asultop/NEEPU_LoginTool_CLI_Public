# NEEPU_LoginTool_CLI_Public
东北电力大学东电NEEPU寝室网登录客户端 （使用 PlainC++ 开发）

## 构建说明 / Build Instructions

### 使用 CMake 构建 / Building with CMake

#### 64位构建 / 64-bit Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### 32位构建 (Linux) / 32-bit Build (Linux)
需要先安装 multilib 支持 / Requires multilib support:
```bash
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install -y gcc-multilib g++-multilib cmake
sudo apt-get install -y libssl-dev:i386 zlib1g-dev:i386 libzstd-dev:i386
```

然后构建 / Then build:
```bash
mkdir build && cd build
PKG_CONFIG_PATH=/usr/lib/i386-linux-gnu/pkgconfig \
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="-m32" \
  -DCMAKE_CXX_FLAGS="-m32" \
  -DCMAKE_EXE_LINKER_FLAGS="-m32 -L/usr/lib/i386-linux-gnu"
cmake --build . --config Release
```

#### 32位构建 (Windows) / 32-bit Build (Windows)
使用 Visual Studio 或 MSVC / Using Visual Studio or MSVC:
```powershell
mkdir build
cd build
cmake .. -A Win32 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### 依赖 / Dependencies
- C++17 编译器 / C++17 compiler
- CMake 3.14+
- CPR (libcurl wrapper) - 自动下载 / automatically fetched
- OpenSSL (用于 HTTPS) / (for HTTPS)

### 使用 / Usage
```bash
./NeepuSTU_Cli --help
./NeepuSTU_Cli --version
./NeepuSTU_Cli -a <账号> -p <密码> -l <运营商>
```

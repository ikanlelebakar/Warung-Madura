# Warung Madura - Sistem Inventaris dan Keuangan

Aplikasi console untuk manajemen inventaris dan keuangan warung, dibuat dengan C++ dan CMake.

## 📋 Requirements

- **CMake** versi 4.0 atau lebih baru
- **C++ Compiler** dengan dukungan C++17 (MinGW, GCC, atau MSVC)
- **Git** untuk clone repository

## 📦 Clone Repository

### Branch Main (CLI Version)

```bash
git clone -b main https://github.com/ikanlelebakar/Warung-Madura.git
cd Warung-Madura
```

### Branch TUI (Text User Interface Version)

```bash
git clone -b TUI https://github.com/ikanlelebakar/Warung-Madura.git
cd Warung-Madura
```

> **Note:** Branch `main` berisi versi CLI standar, sedangkan branch `TUI` menggunakan library FTXUI untuk tampilan Text User Interface yang lebih interaktif.

## 🔧 Build dengan CMake

### 1. Buat folder build

```bash
mkdir build
cd build
```

### 2. Generate build files

**Windows (MinGW):**
```bash
cmake -G "MinGW Makefiles" ..
```

**Windows (Visual Studio):**
```bash
cmake -G "Visual Studio 17 2022" ..
```

**Linux/macOS:**
```bash
cmake ..
```

### 3. Compile project

**MinGW/Linux/macOS:**
```bash
cmake --build .
```

**Atau menggunakan make langsung:**
```bash
make
```

### 4. Jalankan program

**Windows:**
```bash
.\Iventaris_Dan_Keuangan_Konsep.exe
```

**Linux/macOS:**
```bash
./Iventaris_Dan_Keuangan_Konsep
```

## 📁 Struktur Project

```
Warung-Madura/
├── CMakeLists.txt      # Konfigurasi CMake
├── README.md           # Dokumentasi project
├── header/             # Header files (.h)
│   ├── Database.h
│   ├── Kasir.h
│   ├── Stock.h
│   └── PathHelper.h
├── scr/                # Source files (.cpp)
│   ├── Main.cpp
│   ├── Database.cpp
│   ├── Kasir.cpp
│   └── Stock.cpp
├── include/            # External libraries
└── build/              # Build output (generated)
```

## 📝 License

Project ini dibuat untuk keperluan pembelajaran dan pengembangan sistem inventaris warung.

# Warung Madura - Sistem Inventaris dan Keuangan

Aplikasi Text User Interface (TUI) untuk manajemen inventaris dan keuangan warung, dibuat dengan C++ dan FTXUI.

## 📋 Requirements

- **CMake** versi 3.14 atau lebih baru
- **C++ Compiler** dengan dukungan C++17 (MinGW, GCC, atau MSVC)
- **Git** untuk clone repository

## 📦 Clone Repository

```bash
git clone -b TUI https://github.com/ikanlelebakar/Warung-Madura.git
cd Warung-Madura
```

## 🔧 Build dengan CMake

### 1. Buat folder build

```bash
mkdir build
cd build
```

### 2. Generate dan compile

**Windows (MinGW):**
```bash
cmake -G "MinGW Makefiles" ..
cmake --build .
```

**Linux/macOS:**
```bash
cmake ..
cmake --build .
```

### 3. Jalankan program

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
│   ├── PathHelper.h
│   ├── TUI.h
│   ├── TUIComponents.h
│   ├── TUIHelpers.h
│   └── TUIViews.h
├── src/                # Source files (.cpp)
│   ├── Main.cpp
│   ├── Database.cpp
│   ├── Kasir.cpp
│   ├── TUI.cpp
│   ├── TUIComponents.cpp
│   ├── TUIHelpers.cpp
│   └── TUIViews.cpp
├── include/            # External libraries (json.hpp)
├── Database/           # Data files (database.json, transaksi.json)
└── build/              # Build output (generated)
```

## ✨ Fitur

- **KASIR** - Proses belanja, keranjang, checkout
- **STOCK** - Tambah, edit, hapus barang
- **KEUANGAN** - Laporan pemasukan/pengeluaran, export CSV

## 📝 License

Project ini dibuat untuk keperluan pembelajaran dan pengembangan sistem inventaris warung.

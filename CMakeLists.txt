cmake_minimum_required(VERSION 4.0)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking untuk MinGW agar tidak perlu DLL
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Buat folder Database di build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Database)

# Auto-generate database.json dengan isi kosong (hanya jika belum ada)
if(NOT EXISTS ${CMAKE_BINARY_DIR}/Database/database.json)
    file(WRITE ${CMAKE_BINARY_DIR}/Database/database.json
    "{\n    \"barang\": []\n}\n")
    message(STATUS "Created empty database.json")
endif()

# Auto-generate transaksi.json dengan isi kosong (hanya jika belum ada)
if(NOT EXISTS ${CMAKE_BINARY_DIR}/Database/transaksi.json)
    file(WRITE ${CMAKE_BINARY_DIR}/Database/transaksi.json
    "{\n    \"transaksi\": []\n}\n")
    message(STATUS "Created empty transaksi.json")
endif()

add_executable(Iventaris_Dan_Keuangan_Konsep
        scr/Main.cpp
        scr/Kasir.cpp
        scr/Database.cpp
        scr/Stock.cpp
        scr/Keuangan.cpp
        header/Kasir.h
        header/Database.h
        header/Stock.h
        header/PathHelper.h
        header/Keuangan.h
)

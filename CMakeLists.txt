cmake_minimum_required(VERSION 3.16)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking untuk MinGW agar tidak perlu DLL
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    # Link library Windows Socket untuk httplib (diperlukan untuk Windows)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lws2_32")
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

# Copy folder web ke build directory untuk static file serving
file(COPY ${CMAKE_SOURCE_DIR}/web DESTINATION ${CMAKE_BINARY_DIR})

# ============================================================
# TARGET 1: CLI Version (original)
# ============================================================
add_executable(Warung_CLI
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

# ============================================================
# TARGET 2: Web Server Version
# ============================================================
add_executable(Warung_Web
        scr/WebMain.cpp
        scr/ApiServer.cpp
        scr/Database.cpp
        header/ApiServer.h
        header/Database.h
        header/PathHelper.h
)

# Link Windows Socket library untuk httplib (Windows only)
if(WIN32)
    target_link_libraries(Warung_Web ws2_32)
endif()

# Set default target info
message(STATUS "")
message(STATUS "============================================")
message(STATUS "  WARUNG MADURA - Build Targets")
message(STATUS "============================================")
message(STATUS "  Warung_CLI  : CLI version (original)")
message(STATUS "  Warung_Web  : Web server version")
message(STATUS "============================================")
message(STATUS "")

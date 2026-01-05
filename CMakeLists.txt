cmake_minimum_required(VERSION 3.14)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch FTXUI library
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Static linking untuk MinGW agar tidak perlu DLL
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/header)

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
        src/Main.cpp
        src/Kasir.cpp
        src/Database.cpp
        src/TUI.cpp
        src/TUIComponents.cpp
        src/TUIHelpers.cpp
        src/TUIViews.cpp
        header/Kasir.h
        header/Database.h
        header/PathHelper.h
        header/TUI.h
        header/TUIComponents.h
        header/TUIHelpers.h
        header/TUIViews.h
)

target_link_libraries(Iventaris_Dan_Keuangan_Konsep
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

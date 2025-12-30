cmake_minimum_required(VERSION 4.0)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking untuk MinGW agar tidak perlu DLL
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

configure_file(
        ${CMAKE_SOURCE_DIR}/Database/database.json
        ${CMAKE_BINARY_DIR}/Database/database.json
        COPYONLY
)

add_executable(Iventaris_Dan_Keuangan_Konsep
        scr/Main.cpp
        scr/Kasir.cpp
        scr/Database.cpp
        scr/Stock.cpp
        header/Kasir.h
        header/Database.h
        header/Stock.h
        header/PathHelper.h
)

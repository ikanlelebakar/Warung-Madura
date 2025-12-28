cmake_minimum_required(VERSION 4.0)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static linking untuk MinGW agar tidak perlu DLL
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Iventaris_Dan_Keuangan_Konsep
        Main.cpp
        Kasir.cpp
        Kasir.h
        Database.cpp
        Database.h
        Stock.cpp
        Stock.h
        PathHelper.h
)

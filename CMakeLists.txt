cmake_minimum_required(VERSION 4.0)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Iventaris_Dan_Keuangan_Konsep
        Main.cpp
        Kasir.cpp
        Kasir.h
        Database.cpp
        Database.h
        Stock.cpp
        Stock.h
)

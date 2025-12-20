cmake_minimum_required(VERSION 4.0)
project(Iventaris_Dan_Keuangan_Konsep)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Iventaris_Dan_Keuangan_Konsep
        Main.cpp
        Kasir.cpp
        Kasir.h
        Database.cpp
        Database.h
        Stock.cpp
        Stock.h
)

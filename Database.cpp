//
// Created by User on 12/18/2025.
//

#include "Database.h"
#include <iostream>
#include <vector>
#include <iomanip>

std::vector<Database::barang> datasetBarang = {
    {"Ayam", 1000, 10, 20000},
    {"Sapi", 1001, 20, 200000},
    {"Kambing", 1002, 4, 100000},
    {"Kangkung", 1003, 7, 5000},
    {"Coca Cola", 1004, 9, 8000},
};

void Database::tampilBarang() {
    std::cout << std::left
              << std::setw(15) << "Nama Barang"
              << std::setw(15) << "Code Barang"
              << std::setw(18) << "Jumlah Barang"
              << std::setw(15) << "Harga Barang"
              << std::endl;

    std::cout << std::string(63, '-') << std::endl;

    for (const auto& barang : datasetBarang) {
        std::cout << std::left
                  << std::setw(15) << barang.nama
                  << std::setw(15) << barang.codeBarang
                  << std::setw(18) << barang.jumlahBarang
                  << std::setw(15) << barang.hargaBarang
                  << std::endl;
    }
}

// int main() {
//     Database db;
//     db.tampilBarang();
// }
#include "Database.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using json = nlohmann::json;

std::vector<Database::barang> datasetBarang = {
    {"Ayam", 1000, 10, 20000},
    {"Sapi", 1001, 20, 200000},
    {"Kambing", 1002, 4, 100000},
    {"Kangkung", 1003, 7, 5000},
    {"Coca Cola", 1004, 9, 8000},
};

void Database::loadFromJson(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "File JSON tidak ditemukan\n";
        return;
    }

    json j;
    file >> j;
    datasetBarang.clear();

    for (const auto& item : j["barang"]) {
        datasetBarang.push_back({
            item["nama"],
            item["codeBarang"],
            item["jumlahBarang"],
            item["hargaBarang"]
        });
    }
}

void Database::saveToJson(const std::string& fileName) {
    json j;

    for (const auto& b : datasetBarang) {
        j["barang"].push_back({
            {"nama", b.nama},
            {"codeBarang", b.codeBarang},
            {"jumlahBarang", b.jumlahBarang},
            {"hargaBarang", b.hargaBarang}
        });
    }

    std::ofstream file(fileName);
    file << j.dump(4);
}

bool Database::updateStok(int codeBarang, int stokBaru) {
    for (auto& b : datasetBarang) {
        if (b.codeBarang == codeBarang) {
            b.jumlahBarang = stokBaru;
            return true;
        }
    }
    return false;
}

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
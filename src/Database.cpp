#include "Database.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using json = nlohmann::json;
std::vector<Database::barang> datasetBarang;

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

void Database::initDatabase(const std::string& fileName) {
    if (std::filesystem::exists(fileName)) {
        return;
    }

    json j;
    j["barang"] = json::array();

    std::ofstream file(fileName);
    file << j.dump(4);

    std::cout << "database.json berhasil dibuat otomatis\n";
}

void Database::reindexCodes() {
    int newCode = 1000;
    for (auto& b : datasetBarang) {
        b.codeBarang = newCode++;
    }
}
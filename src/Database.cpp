#include "Database.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using json = nlohmann::json;

// Dataset global untuk barang dan transaksi
std::vector<Database::barang> datasetBarang;
std::vector<Database::Transaksi> datasetTransaksi;

// ============================================================
// FUNGSI LOAD & SAVE BARANG
// ============================================================

void Database::loadFromJson(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
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
}

void Database::reindexCodes() {
    int newCode = 1000;
    for (auto& b : datasetBarang) {
        b.codeBarang = newCode++;
    }
}

// ============================================================
// FUNGSI LOAD & SAVE TRANSAKSI
// ============================================================

void Database::loadTransaksi(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return;
    }

    json j;
    file >> j;
    datasetTransaksi.clear();

    if (j.contains("transaksi")) {
        for (const auto& item : j["transaksi"]) {
            Transaksi t;
            t.id = item["id"];
            t.tanggal = item["tanggal"];
            t.waktu = item["waktu"];
            t.jenis = item["jenis"];
            t.keterangan = item["keterangan"];
            t.jumlah = item["jumlah"];
            
            // Backward compatibility
            if (item.contains("metodePembayaran")) {
                t.metodePembayaran = item["metodePembayaran"];
            } else {
                t.metodePembayaran = "tidak diketahui";
            }
            
            datasetTransaksi.push_back(t);
        }
    }
}

void Database::saveTransaksi(const std::string& fileName) {
    json j;
    j["transaksi"] = json::array();

    for (const auto& t : datasetTransaksi) {
        j["transaksi"].push_back({
            {"id", t.id},
            {"tanggal", t.tanggal},
            {"waktu", t.waktu},
            {"jenis", t.jenis},
            {"keterangan", t.keterangan},
            {"jumlah", t.jumlah},
            {"metodePembayaran", t.metodePembayaran}
        });
    }

    std::ofstream file(fileName);
    file << j.dump(4);
}

void Database::initTransaksi(const std::string& fileName) {
    if (std::filesystem::exists(fileName)) {
        return;
    }

    json j;
    j["transaksi"] = json::array();

    std::ofstream file(fileName);
    file << j.dump(4);
}

void Database::tambahTransaksi(const Transaksi& t) {
    datasetTransaksi.push_back(t);
}
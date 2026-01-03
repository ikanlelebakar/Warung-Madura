#include "../header/Database.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <sstream>

using json = nlohmann::json;
std::vector<Database::barang> datasetBarang;
std::vector<Database::Transaksi> datasetTransaksi;

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

void Database::initDatabase(const std::string& fileName) {
    if (std::filesystem::exists(fileName)) {
        return; // database sudah ada
    }

    json j;
    j["barang"] = json::array(); // database kosong tapi valid

    std::ofstream file(fileName);
    file << j.dump(4);

    std::cout << "database.json berhasil dibuat otomatis\n";
}

// ==================== TRANSAKSI FUNCTIONS ====================

void Database::loadTransaksi(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return; // File tidak ada, tidak error
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
            // Backward compatibility: old data may not have metodePembayaran
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
        return; // file sudah ada
    }

    json j;
    j["transaksi"] = json::array();

    std::ofstream file(fileName);
    file << j.dump(4);

    std::cout << "transaksi.json berhasil dibuat otomatis\n";
}

void Database::tambahTransaksi(const Transaksi& t) {
    datasetTransaksi.push_back(t);
}
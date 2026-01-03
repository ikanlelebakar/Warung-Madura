/**
 * ============================================================
 * MODUL DATABASE - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : Database.cpp
 * Deskripsi   : Modul untuk mengelola penyimpanan dan pembacaan
 *               data barang dan transaksi dari/ke file JSON
 * ============================================================
 */

#include "../header/Database.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <sstream>

// Menggunakan alias untuk library JSON (nlohmann/json)
using json = nlohmann::json;

// ============================================================
// VARIABEL GLOBAL DATASET
// ============================================================

/**
 * Dataset untuk menyimpan data barang
 * Diakses oleh modul lain (Kasir, Stock)
 */
std::vector<Database::barang> datasetBarang;

/**
 * Dataset untuk menyimpan data transaksi
 * Diakses oleh modul lain (Kasir, Stock, Keuangan)
 */
std::vector<Database::Transaksi> datasetTransaksi;

// ============================================================
// FUNGSI LOAD & SAVE BARANG (FROM/TO JSON)
// ============================================================

/**
 * Fungsi untuk memuat data barang dari file JSON
 * Data yang dibaca akan disimpan ke datasetBarang
 * @param fileName - path ke file JSON
 */
void Database::loadFromJson(const std::string& fileName) {
    std::ifstream file(fileName);
    
    // Cek apakah file berhasil dibuka
    if (!file.is_open()) {
        std::cout << "  [!] File JSON tidak ditemukan" << std::endl;
        return;
    }

    json j;
    file >> j;  // Parse JSON dari file
    
    // Kosongkan dataset sebelum mengisi ulang
    datasetBarang.clear();

    // Loop setiap item barang dalam JSON dan tambahkan ke dataset
    for (const auto& item : j["barang"]) {
        datasetBarang.push_back({
            item["nama"],           // Nama barang
            item["codeBarang"],     // Code unik barang
            item["jumlahBarang"],   // Jumlah stok
            item["hargaBarang"]     // Harga satuan
        });
    }
}

/**
 * Fungsi untuk menyimpan data barang ke file JSON
 * Data dari datasetBarang akan ditulis ke file
 * @param fileName - path ke file JSON
 */
void Database::saveToJson(const std::string& fileName) {
    json j;

    // Konversi setiap barang di dataset ke format JSON
    for (const auto& b : datasetBarang) {
        j["barang"].push_back({
            {"nama", b.nama},
            {"codeBarang", b.codeBarang},
            {"jumlahBarang", b.jumlahBarang},
            {"hargaBarang", b.hargaBarang}
        });
    }

    // Tulis JSON ke file dengan indentasi 4 spasi
    std::ofstream file(fileName);
    file << j.dump(4);
}

// ============================================================
// FUNGSI UPDATE STOK
// ============================================================

/**
 * Fungsi untuk mengupdate stok barang berdasarkan code
 * @param codeBarang - code barang yang akan diupdate
 * @param stokBaru - jumlah stok baru
 * @return bool - true jika berhasil, false jika barang tidak ditemukan
 */
bool Database::updateStok(int codeBarang, int stokBaru) {
    for (auto& b : datasetBarang) {
        if (b.codeBarang == codeBarang) {
            b.jumlahBarang = stokBaru;
            return true;
        }
    }
    return false;
}

// ============================================================
// FUNGSI TAMPIL BARANG
// ============================================================

/**
 * Fungsi untuk menampilkan daftar barang dalam format tabel
 * Menampilkan: Nama, Code, Jumlah, dan Harga
 */
void Database::tampilBarang() {
    // Header kolom
    std::cout << std::left
              << std::setw(15) << "Nama Barang"
              << std::setw(15) << "Code Barang"
              << std::setw(18) << "Jumlah Barang"
              << std::setw(15) << "Harga Barang"
              << std::endl;

    std::cout << std::string(63, '-') << std::endl;

    // Data rows
    for (const auto& barang : datasetBarang) {
        std::cout << std::left
                  << std::setw(15) << barang.nama
                  << std::setw(15) << barang.codeBarang
                  << std::setw(18) << barang.jumlahBarang
                  << std::setw(15) << std::fixed << std::setprecision(0) << barang.hargaBarang
                  << std::endl;
    }
}

// ============================================================
// FUNGSI INISIALISASI DATABASE
// ============================================================

/**
 * Fungsi untuk menginisialisasi file database.json
 * Jika file belum ada, akan dibuat file kosong yang valid
 * @param fileName - path ke file database.json
 */
void Database::initDatabase(const std::string& fileName) {
    // Cek apakah file sudah ada
    if (std::filesystem::exists(fileName)) {
        return; // Database sudah ada, tidak perlu buat baru
    }

    // Buat JSON kosong dengan struktur yang valid
    json j;
    j["barang"] = json::array();

    // Tulis ke file
    std::ofstream file(fileName);
    file << j.dump(4);

    std::cout << "  [i] database.json berhasil dibuat otomatis" << std::endl;
}

// ============================================================
// FUNGSI LOAD TRANSAKSI
// ============================================================

/**
 * Fungsi untuk memuat data transaksi dari file JSON
 * Mendukung backward compatibility untuk data lama
 * @param fileName - path ke file transaksi.json
 */
void Database::loadTransaksi(const std::string& fileName) {
    std::ifstream file(fileName);
    
    // Jika file tidak ada, tidak error (transaksi mungkin belum ada)
    if (!file.is_open()) {
        return;
    }

    json j;
    file >> j;
    
    // Kosongkan dataset sebelum mengisi ulang
    datasetTransaksi.clear();

    // Cek apakah key "transaksi" ada dalam JSON
    if (j.contains("transaksi")) {
        for (const auto& item : j["transaksi"]) {
            Transaksi t;
            t.id = item["id"];
            t.tanggal = item["tanggal"];
            t.waktu = item["waktu"];
            t.jenis = item["jenis"];
            t.keterangan = item["keterangan"];
            t.jumlah = item["jumlah"];
            
            // Backward compatibility: data lama mungkin tidak punya metodePembayaran
            if (item.contains("metodePembayaran")) {
                t.metodePembayaran = item["metodePembayaran"];
            } else {
                t.metodePembayaran = "tidak diketahui";
            }
            
            datasetTransaksi.push_back(t);
        }
    }
}

// ============================================================
// FUNGSI SAVE TRANSAKSI
// ============================================================

/**
 * Fungsi untuk menyimpan data transaksi ke file JSON
 * @param fileName - path ke file transaksi.json
 */
void Database::saveTransaksi(const std::string& fileName) {
    json j;
    j["transaksi"] = json::array();

    // Konversi setiap transaksi ke format JSON
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

    // Tulis ke file dengan indentasi 4 spasi
    std::ofstream file(fileName);
    file << j.dump(4);
}

// ============================================================
// FUNGSI INISIALISASI TRANSAKSI
// ============================================================

/**
 * Fungsi untuk menginisialisasi file transaksi.json
 * Jika file belum ada, akan dibuat file kosong yang valid
 * @param fileName - path ke file transaksi.json
 */
void Database::initTransaksi(const std::string& fileName) {
    // Cek apakah file sudah ada
    if (std::filesystem::exists(fileName)) {
        return; // File sudah ada
    }

    // Buat JSON kosong dengan struktur yang valid
    json j;
    j["transaksi"] = json::array();

    // Tulis ke file
    std::ofstream file(fileName);
    file << j.dump(4);

    std::cout << "  [i] transaksi.json berhasil dibuat otomatis" << std::endl;
}

// ============================================================
// FUNGSI TAMBAH TRANSAKSI
// ============================================================

/**
 * Fungsi untuk menambahkan transaksi baru ke dataset
 * @param t - objek Transaksi yang akan ditambahkan
 */
void Database::tambahTransaksi(const Transaksi& t) {
    datasetTransaksi.push_back(t);
}
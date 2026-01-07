/**
 * ============================================================
 * MAIN WEB SERVER - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : WebMain.cpp
 * Deskripsi   : Entry point untuk versi web
 *               Menjalankan HTTP server yang serve frontend 
 *               dan API endpoints
 * ============================================================
 * 
 * PERBEDAAN DENGAN Main.cpp (CLI):
 * - Main.cpp: menjalankan menu CLI berbasis cout/cin
 * - WebMain.cpp: menjalankan HTTP server untuk web UI
 * 
 * Kedua file bisa di-compile terpisah via CMake target
 * ============================================================
 */

// === HEADER MODUL INTERNAL ===
#include "../header/ApiServer.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"

// === STANDARD LIBRARY ===
#include <iostream>

/**
 * Entry point untuk web server
 * Inisialisasi database dan jalankan HTTP server
 */
int main() {
    // Inisialisasi database jika belum ada
    Database database;
    database.initDatabase(getDatabasePath());
    database.initTransaksi(getTransaksiPath());
    
    // Load data awal
    database.loadFromJson(getDatabasePath());
    database.loadTransaksi(getTransaksiPath());
    
    std::cout << std::endl;
    std::cout << "  Loaded " << datasetBarang.size() << " barang" << std::endl;
    std::cout << "  Loaded " << datasetTransaksi.size() << " transaksi" << std::endl;
    std::cout << std::endl;
    
    // Jalankan API server
    ApiServer apiServer;
    apiServer.start(8080);
    
    return 0;
}

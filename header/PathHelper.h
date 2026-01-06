#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H

// === STANDARD LIBRARY ===
#include <string>      // std::string untuk path dan return value fungsi
#include <filesystem>  // std::filesystem untuk manipulasi path (fs::path, fs::exists)
#include <chrono>      // std::chrono untuk mendapatkan timestamp saat ini
#include <sstream>     // std::ostringstream untuk memformat tanggal/waktu/ID transaksi
#include <iomanip>     // setw, setfill untuk format tanggal DD-MM-YYYY
#include <limits>      // numeric_limits untuk cin.ignore() dalam validasi input
#include <iostream>    // cout/cin untuk fungsi getValidInt, getValidIntInput, getValidDouble

// === WINDOWS-SPECIFIC HEADERS ===
#ifdef _WIN32
#include <windows.h>   // GetModuleFileNameA() untuk mendapatkan path executable
#include <shlobj.h>    // SHGetFolderPathA() untuk mendapatkan path folder Documents user
#endif

namespace fs = std::filesystem;

inline std::string getDatabasePath() {
    fs::path exePath;
    
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    exePath = fs::path(buffer).parent_path();
#else
    exePath = fs::current_path();
#endif
    
    fs::path dbPath;
    
    // Coba path dari cmake-build-debug (../Database/database.json)
    dbPath = exePath.parent_path() / "Database" / "database.json";
    if (fs::exists(dbPath.parent_path())) {
        return dbPath.string();
    }
    
    // Coba path dari folder root project (Database/database.json)
    dbPath = exePath / "Database" / "database.json";
    if (fs::exists(dbPath.parent_path())) {
        return dbPath.string();
    }
    
    // Fallback ke current working directory
    dbPath = fs::current_path() / "Database" / "database.json";
    return dbPath.string();
}

inline std::string getTransaksiPath() {
    fs::path exePath;
    
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    exePath = fs::path(buffer).parent_path();
#else
    exePath = fs::current_path();
#endif
    
    fs::path tPath;
    
    // Coba path dari cmake-build-debug (../Database/transaksi.json)
    tPath = exePath.parent_path() / "Database" / "transaksi.json";
    if (fs::exists(tPath.parent_path())) {
        return tPath.string();
    }
    
    // Coba path dari folder root project (Database/transaksi.json)
    tPath = exePath / "Database" / "transaksi.json";
    if (fs::exists(tPath.parent_path())) {
        return tPath.string();
    }
    
    // Fallback ke current working directory
    tPath = fs::current_path() / "Database" / "transaksi.json";
    return tPath.string();
}

inline std::string getDocumentsPath() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
        return std::string(path);
    }
#endif
    // Fallback ke current directory
    return fs::current_path().string();
}

// Helper function untuk mendapatkan tanggal saat ini (format: DD-MM-YYYY)
inline std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << tm_now.tm_mday << "-"
        << std::setfill('0') << std::setw(2) << (tm_now.tm_mon + 1) << "-"
        << (tm_now.tm_year + 1900);
    return oss.str();
}

// Helper function untuk mendapatkan waktu saat ini (format: HH:MM:SS)
inline std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << tm_now.tm_hour << ":"
        << std::setfill('0') << std::setw(2) << tm_now.tm_min << ":"
        << std::setfill('0') << std::setw(2) << tm_now.tm_sec;
    return oss.str();
}

// Helper function untuk generate transaction ID (timestamp-based)
inline std::string generateTransactionId() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    
    std::ostringstream oss;
    oss << "TRX" << millis;
    return oss.str();
}

// Helper function untuk mendapatkan bulan dan tahun saat ini (format: YYYY_MM)
inline std::string getCurrentMonthYear() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif
    
    std::ostringstream oss;
    oss << (tm_now.tm_year + 1900) << "_"
        << std::setfill('0') << std::setw(2) << (tm_now.tm_mon + 1);
    return oss.str();
}

// ============================================================
// FUNGSI INPUT VALIDATION
// ============================================================

/**
 * Fungsi untuk mendapatkan input integer yang valid dari user
 * Menangani fail state dan buffer secara otomatis
 * SOLUSI untuk bug infinite loop pada cin >> int
 * 
 * @param prompt - pesan yang ditampilkan ke user
 * @param min - nilai minimum yang valid
 * @param max - nilai maksimum yang valid
 * @return int - nilai integer yang valid dalam range [min, max]
 */
inline int getValidIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        // Cek apakah input berhasil (tidak fail)
        if (std::cin.fail()) {
            // Reset fail state agar cin bisa digunakan lagi
            std::cin.clear();
            // Kosongkan buffer sampai newline (buang input yang salah)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Input harus berupa angka!" << std::endl;
        } 
        // Cek apakah nilai dalam range yang valid
        else if (value < min || value > max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Input harus antara " << min << " dan " << max << std::endl;
        } 
        // Input valid, kembalikan nilai
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

/**
 * Fungsi untuk mendapatkan input integer tanpa batasan range
 * Cocok untuk input seperti code barang atau jumlah yang bisa berapa saja
 * 
 * @param prompt - pesan yang ditampilkan ke user
 * @return int - nilai integer yang valid
 */
inline int getValidInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Input harus berupa angka!" << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

/**
 * Fungsi untuk mendapatkan input double yang valid (untuk harga)
 * 
 * @param prompt - pesan yang ditampilkan ke user
 * @return double - nilai double yang valid
 */
inline double getValidDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Input harus berupa angka!" << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

// ============================================================
// FUNGSI PERHITUNGAN HARGA JUAL (MARKUP)
// ============================================================

/**
 * Fungsi untuk menghitung harga jual berdasarkan harga beli (modal)
 * Markup otomatis:
 * - Harga beli >= 10.000 → markup +2.000
 * - Harga beli < 10.000  → markup +1.000
 * 
 * @param hargaBeli - harga beli/modal barang
 * @return double - harga jual setelah markup
 */
inline double hitungHargaJual(double hargaBeli) {
    if (hargaBeli >= 10000) {
        return hargaBeli + 2000;  // Markup Rp 2.000 untuk barang >= 10rb
    } else {
        return hargaBeli + 1000;  // Markup Rp 1.000 untuk barang < 10rb
    }
}

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H

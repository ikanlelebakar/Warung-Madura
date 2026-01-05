#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H

#include <string>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

namespace fs = std::filesystem;

// ============================================================
// PATH FUNCTIONS
// ============================================================

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
    
    dbPath = exePath.parent_path() / "Database" / "database.json";
    if (fs::exists(dbPath.parent_path())) {
        return dbPath.string();
    }
    
    dbPath = exePath / "Database" / "database.json";
    if (fs::exists(dbPath.parent_path())) {
        return dbPath.string();
    }
    
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
    
    tPath = exePath.parent_path() / "Database" / "transaksi.json";
    if (fs::exists(tPath.parent_path())) {
        return tPath.string();
    }
    
    tPath = exePath / "Database" / "transaksi.json";
    if (fs::exists(tPath.parent_path())) {
        return tPath.string();
    }
    
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
    return fs::current_path().string();
}

// ============================================================
// DATE/TIME FUNCTIONS
// ============================================================

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

inline std::string generateTransactionId() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    
    std::ostringstream oss;
    oss << "TRX" << millis;
    return oss.str();
}

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

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H

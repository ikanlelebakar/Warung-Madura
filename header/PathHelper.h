#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H

#include <string>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
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

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_PATHHELPER_H

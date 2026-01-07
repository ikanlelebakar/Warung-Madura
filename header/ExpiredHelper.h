#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_EXPIREDHELPER_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_EXPIREDHELPER_H

/**
 * ============================================================
 * HELPER EXPIRED DATE - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : ExpiredHelper.h
 * Deskripsi   : Helper untuk logika tanggal expired dan warna
 *               terminal menggunakan ANSI escape code
 * ============================================================
 */

// === STANDARD LIBRARY ===
#include <string>    // std::string untuk return value
#include <chrono>    // std::chrono untuk mendapatkan waktu sistem
#include <ctime>     // struct tm untuk manipulasi tanggal

// ============================================================
// KONSTANTA ANSI COLOR CODE
// ============================================================

/**
 * ANSI escape code untuk warna terminal
 * Digunakan untuk menampilkan teks berwarna di CLI
 */
const std::string ANSI_RED    = "\033[31m";   // Warna merah untuk expired
const std::string ANSI_YELLOW = "\033[33m";   // Warna kuning (opsional)
const std::string ANSI_RESET  = "\033[0m";    // Reset ke warna default

// ============================================================
// KONSTANTA PERINGATAN EXPIRED
// ============================================================

/**
 * Jumlah hari sebelum expired untuk menampilkan peringatan
 * Jika (tanggal sekarang >= tanggal expired - N_DAYS_WARNING)
 * maka barang akan ditampilkan berwarna merah
 */
const int N_DAYS_WARNING = 7;  // 7 hari sebelum expired

// ============================================================
// FUNGSI HELPER EXPIRED DATE
// ============================================================

/**
 * Mengkonversi tanggal (hari, bulan, tahun) ke jumlah hari sejak epoch
 * Digunakan untuk membandingkan dua tanggal dengan mudah
 * 
 * @param day   - hari (1-31)
 * @param month - bulan (1-12)
 * @param year  - tahun (e.g. 2026)
 * @return long long - jumlah hari sejak epoch (1970-01-01)
 */
inline long long dateToDays(int day, int month, int year) {
    std::tm time = {};
    time.tm_mday = day;
    time.tm_mon = month - 1;      // tm_mon is 0-based (0 = January)
    time.tm_year = year - 1900;   // tm_year is years since 1900
    time.tm_hour = 12;            // Noon to avoid timezone issues
    
    // Konversi ke time_t lalu ke hari
    std::time_t t = std::mktime(&time);
    return t / (60 * 60 * 24);    // Konversi detik ke hari
}

/**
 * Mendapatkan tanggal hari ini dalam format jumlah hari sejak epoch
 * 
 * @return long long - jumlah hari sejak epoch untuk hari ini
 */
inline long long getTodayInDays() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif
    
    return dateToDays(tm_now.tm_mday, tm_now.tm_mon + 1, tm_now.tm_year + 1900);
}

/**
 * Mengecek apakah barang mendekati tanggal expired (BUKAN sudah expired)
 * Barang dianggap "mendekati expired" jika:
 * - Belum melewati tanggal expired (todayDays <= expiredDays)
 * - DAN selisih hari <= N_DAYS_WARNING
 * 
 * @param expDay   - hari expired
 * @param expMonth - bulan expired
 * @param expYear  - tahun expired
 * @return bool    - true HANYA jika mendekati expired (bukan sudah lewat)
 */
inline bool isNearExpired(int expDay, int expMonth, int expYear) {
    // Jika tanggal expired tidak valid (0), anggap tidak expired
    if (expDay == 0 || expMonth == 0 || expYear == 0) {
        return false;
    }
    
    long long todayDays = getTodayInDays();
    long long expiredDays = dateToDays(expDay, expMonth, expYear);
    long long selisih = expiredDays - todayDays;
    
    // HANYA mendekati expired jika:
    // 1. Belum lewat (selisih >= 0)
    // 2. Selisih <= N_DAYS_WARNING
    return (selisih >= 0 && selisih <= N_DAYS_WARNING);
}

/**
 * Mengecek apakah barang sudah melewati tanggal expired
 * 
 * @param expDay   - hari expired
 * @param expMonth - bulan expired
 * @param expYear  - tahun expired
 * @return bool    - true jika sudah expired (tanggal sekarang > expired)
 */
inline bool isExpired(int expDay, int expMonth, int expYear) {
    if (expDay == 0 || expMonth == 0 || expYear == 0) {
        return false;
    }
    
    long long todayDays = getTodayInDays();
    long long expiredDays = dateToDays(expDay, expMonth, expYear);
    
    return todayDays > expiredDays;
}

/**
 * Enum untuk status expired barang
 */
enum class ExpiredStatus {
    SAFE,         // Masih aman (> N_DAYS_WARNING dari expired)
    NEAR_EXPIRED, // Mendekati expired (<= N_DAYS_WARNING)
    EXPIRED       // Sudah melewati tanggal expired
};

/**
 * Mendapatkan status expired barang
 * 
 * @param expDay   - hari expired
 * @param expMonth - bulan expired
 * @param expYear  - tahun expired
 * @return ExpiredStatus - SAFE, NEAR_EXPIRED, atau EXPIRED
 */
inline ExpiredStatus getExpiredStatus(int expDay, int expMonth, int expYear) {
    if (expDay == 0 || expMonth == 0 || expYear == 0) {
        return ExpiredStatus::SAFE;
    }
    
    if (isExpired(expDay, expMonth, expYear)) {
        return ExpiredStatus::EXPIRED;
    }
    
    if (isNearExpired(expDay, expMonth, expYear)) {
        return ExpiredStatus::NEAR_EXPIRED;
    }
    
    return ExpiredStatus::SAFE;
}

/**
 * Mengembalikan nama barang dengan warna ANSI berdasarkan status expired
 * - EXPIRED → nama MERAH + label "[!]"
 * - NEAR_EXPIRED → nama KUNING
 * - SAFE → nama normal (tanpa warna)
 * 
 * @param nama     - nama barang
 * @param expDay   - hari expired
 * @param expMonth - bulan expired
 * @param expYear  - tahun expired
 * @return string  - nama dengan atau tanpa ANSI color code
 */
inline std::string getColoredName(const std::string& nama, int expDay, int expMonth, int expYear) {
    ExpiredStatus status = getExpiredStatus(expDay, expMonth, expYear);
    
    switch (status) {
        case ExpiredStatus::EXPIRED:
            return ANSI_RED + nama + ANSI_RESET;
        case ExpiredStatus::NEAR_EXPIRED:
            return ANSI_YELLOW + nama + ANSI_RESET;
        case ExpiredStatus::SAFE:
        default:
            return nama;
    }
}

/**
 * Mendapatkan panjang ANSI escape code untuk padding
 * 
 * @param expDay   - hari expired
 * @param expMonth - bulan expired
 * @param expYear  - tahun expired
 * @return int     - panjang ANSI code (untuk kompensasi setw)
 */
inline int getAnsiPadding(int expDay, int expMonth, int expYear) {
    ExpiredStatus status = getExpiredStatus(expDay, expMonth, expYear);
    
    if (status == ExpiredStatus::EXPIRED || status == ExpiredStatus::NEAR_EXPIRED) {
        // ANSI_RED/YELLOW + ANSI_RESET = total karakter escape
        return ANSI_RED.length() + ANSI_RESET.length();
    }
    return 0;
}

/**
 * Format tanggal expired ke string DD-MM-YYYY dengan status
 * - Jika sudah expired, tambahkan "[X]"
 * - Jika mendekati expired, tambahkan "[!]"
 * 
 * @param expDay   - hari expired
 * @param expMonth - bulan expired
 * @param expYear  - tahun expired
 * @return string  - tanggal dalam format "DD-MM-YYYY" + status
 */
inline std::string formatExpiredDate(int expDay, int expMonth, int expYear) {
    if (expDay == 0 || expMonth == 0 || expYear == 0) {
        return "-";
    }
    
    std::string day = (expDay < 10 ? "0" : "") + std::to_string(expDay);
    std::string month = (expMonth < 10 ? "0" : "") + std::to_string(expMonth);
    std::string year = std::to_string(expYear);
    std::string dateStr = day + "-" + month + "-" + year;
    
    ExpiredStatus status = getExpiredStatus(expDay, expMonth, expYear);
    
    switch (status) {
        case ExpiredStatus::EXPIRED:
            return ANSI_RED + dateStr + " [X]" + ANSI_RESET;
        case ExpiredStatus::NEAR_EXPIRED:
            return ANSI_YELLOW + dateStr + " [!]" + ANSI_RESET;
        default:
            return dateStr;
    }
}

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_EXPIREDHELPER_H

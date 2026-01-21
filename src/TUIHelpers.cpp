#include "TUIHelpers.h"
#include "Database.h"
#include "Kasir.h"
#include "PathHelper.h"
#include "ExpiredHelper.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>

namespace TUI {

std::string formatRupiah(double value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << value;
    return "Rp " + ss.str();
}

std::vector<std::vector<std::string>> getStockTableData() {
    std::vector<std::vector<std::string>> data;
    for (const auto& b : datasetBarang) {
        data.push_back({
            b.nama, 
            std::to_string(b.codeBarang), 
            std::to_string(b.jumlahBarang), 
            formatRupiah(b.hargaBarang)
        });
    }
    return data;
}

std::vector<std::vector<std::string>> getCartTableData() {
    std::vector<std::vector<std::string>> data;
    for (const auto& item : keranjang) {
        double subtotal = item.harga * item.jumlah;
        data.push_back({
            item.namaBarang,
            std::to_string(item.codeBarang),
            std::to_string(item.jumlah),
            formatRupiah(item.harga),
            formatRupiah(subtotal)
        });
    }
    return data;
}

double getCartTotal() {
    double total = 0;
    for (const auto& item : keranjang) {
        total += item.harga * item.jumlah;
    }
    return total;
}

// ============================================================
// KEUANGAN FUNCTIONS
// ============================================================

double hitungTotalPemasukan() {
    double total = 0;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            total += t.jumlah;
        }
    }
    return total;
}

double hitungTotalPengeluaran() {
    double total = 0;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pengeluaran") {
            total += t.jumlah;
        }
    }
    return total;
}

double hitungLabaBersih() {
    return hitungTotalPemasukan() - hitungTotalPengeluaran();
}

std::vector<std::vector<std::string>> getPemasukanTableData() {
    std::vector<Database::Transaksi> pemasukan;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            pemasukan.push_back(t);
        }
    }
    
    // Sort descending by date+time (terbaru di atas)
    std::sort(pemasukan.begin(), pemasukan.end(),
        [](const Database::Transaksi& a, const Database::Transaksi& b) {
            std::string dateA = a.tanggal.substr(6,4) + a.tanggal.substr(3,2) + a.tanggal.substr(0,2) + a.waktu;
            std::string dateB = b.tanggal.substr(6,4) + b.tanggal.substr(3,2) + b.tanggal.substr(0,2) + b.waktu;
            return dateA > dateB;
        });
    
    std::vector<std::vector<std::string>> data;
    for (const auto& t : pemasukan) {
        data.push_back({
            t.tanggal,
            t.waktu,
            t.keterangan,
            t.metodePembayaran,
            formatRupiah(t.jumlah)
        });
    }
    return data;
}

std::vector<std::vector<std::string>> getPengeluaranTableData() {
    std::vector<Database::Transaksi> pengeluaran;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pengeluaran") {
            pengeluaran.push_back(t);
        }
    }
    
    // Sort descending
    std::sort(pengeluaran.begin(), pengeluaran.end(),
        [](const Database::Transaksi& a, const Database::Transaksi& b) {
            std::string dateA = a.tanggal.substr(6,4) + a.tanggal.substr(3,2) + a.tanggal.substr(0,2) + a.waktu;
            std::string dateB = b.tanggal.substr(6,4) + b.tanggal.substr(3,2) + b.tanggal.substr(0,2) + b.waktu;
            return dateA > dateB;
        });
    
    std::vector<std::vector<std::string>> data;
    for (const auto& t : pengeluaran) {
        data.push_back({
            t.tanggal,
            t.waktu,
            t.keterangan,
            formatRupiah(t.jumlah)
        });
    }
    return data;
}

std::string exportToCSV() {
    std::string fileName = "Laporan_Keuangan_" + getCurrentMonthYear() + ".csv";
    std::string fullPath = getDocumentsPath() + "\\" + fileName;
    
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        return "";
    }
    
    // Header CSV
    file << "ID,Tanggal,Waktu,Jenis,Keterangan,Jumlah,MetodePembayaran\n";
    
    // Data transaksi
    for (const auto& t : datasetTransaksi) {
        file << t.id << ","
             << t.tanggal << ","
             << t.waktu << ","
             << t.jenis << ","
             << "\"" << t.keterangan << "\","
             << std::fixed << std::setprecision(0) << t.jumlah << ","
             << t.metodePembayaran << "\n";
    }
    
    // Ringkasan
    file << "\n=== RINGKASAN ===\n";
    file << "Total Pemasukan,,,," << hitungTotalPemasukan() << "\n";
    file << "Total Pengeluaran,,,," << hitungTotalPengeluaran() << "\n";
    file << "Laba Bersih,,,," << hitungLabaBersih() << "\n";
    
    file.close();
    return fullPath;
}

// ============================================================
// EXPIRED DATE HELPERS
// ============================================================

TUIExpiredStatus getTUIExpiredStatus(int expDay, int expMonth, int expYear) {
    // Map from ExpiredHelper's logic to TUI status
    if (expDay == 0 || expMonth == 0 || expYear == 0) {
        return TUIExpiredStatus::SAFE;
    }
    
    if (isExpired(expDay, expMonth, expYear)) {
        return TUIExpiredStatus::EXPIRED;
    }
    
    if (isNearExpired(expDay, expMonth, expYear)) {
        return TUIExpiredStatus::NEAR_EXPIRED;
    }
    
    return TUIExpiredStatus::SAFE;
}

std::string formatExpiredDateTUI(int expDay, int expMonth, int expYear) {
    if (expDay == 0 || expMonth == 0 || expYear == 0) {
        return "-";
    }
    
    std::string day = (expDay < 10 ? "0" : "") + std::to_string(expDay);
    std::string month = (expMonth < 10 ? "0" : "") + std::to_string(expMonth);
    std::string year = std::to_string(expYear);
    std::string dateStr = day + "-" + month + "-" + year;
    
    TUIExpiredStatus status = getTUIExpiredStatus(expDay, expMonth, expYear);
    
    switch (status) {
        case TUIExpiredStatus::EXPIRED:
            return dateStr + " [X]";
        case TUIExpiredStatus::NEAR_EXPIRED:
            return dateStr + " [!]";
        default:
            return dateStr;
    }
}

// ============================================================
// FLASH SALE IMPLEMENTATION
// ============================================================

// Flash sale discount constants (midpoints of specified ranges)
const double FLASH_SALE_DISCOUNT_7_DAYS = 11.5;   // <= 7 days: 11-12% -> 11.5%
const double FLASH_SALE_DISCOUNT_14_DAYS = 7.5;   // <= 14 days: 5-10% -> 7.5%
const double FLASH_SALE_DISCOUNT_21_DAYS = 14.5;  // <= 21 days: 13-16% -> 14.5%

FlashSaleInfo getFlashSaleInfo(const Database::barang& b) {
    FlashSaleInfo info;
    info.effectivePrice = b.hargaJual;  // Default: no discount
    info.discountPercent = 0.0;
    info.isOnSale = false;
    info.daysUntilExpired = getDaysUntilExpired(b.expDay, b.expMonth, b.expYear);
    
    // No flash sale for:
    // - Items without expiration date (daysUntilExpired == LLONG_MAX)
    // - Items already expired (daysUntilExpired <= 0)
    // - Items with > 21 days until expiration
    if (info.daysUntilExpired == LLONG_MAX || info.daysUntilExpired <= 0) {
        return info;
    }
    
    // Determine discount tier based on days until expiration
    if (info.daysUntilExpired <= 7) {
        // Tier 1: 1-7 days -> 11.5% discount
        info.discountPercent = FLASH_SALE_DISCOUNT_7_DAYS;
        info.isOnSale = true;
    } else if (info.daysUntilExpired <= 14) {
        // Tier 2: 8-14 days -> 7.5% discount
        info.discountPercent = FLASH_SALE_DISCOUNT_14_DAYS;
        info.isOnSale = true;
    } else if (info.daysUntilExpired <= 21) {
        // Tier 3: 15-21 days -> 14.5% discount
        info.discountPercent = FLASH_SALE_DISCOUNT_21_DAYS;
        info.isOnSale = true;
    }
    // else: > 21 days, no discount
    
    // Calculate effective price if on sale
    if (info.isOnSale) {
        double discountMultiplier = (100.0 - info.discountPercent) / 100.0;
        info.effectivePrice = b.hargaJual * discountMultiplier;
        
        // Round to nearest integer for cleaner pricing
        info.effectivePrice = std::round(info.effectivePrice);
    }
    
    return info;
}

double getEffectivePrice(const Database::barang& b) {
    return getFlashSaleInfo(b).effectivePrice;
}

bool isFlashSaleEligible(const Database::barang& b) {
    return getFlashSaleInfo(b).isOnSale;
}

} // namespace TUI

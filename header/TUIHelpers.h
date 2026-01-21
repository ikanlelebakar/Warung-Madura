#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_HELPERS_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_HELPERS_H

#include "Database.h"
#include <string>
#include <vector>

namespace TUI {

// Format angka ke format Rupiah
std::string formatRupiah(double value);

// Get data untuk tabel stock
std::vector<std::vector<std::string>> getStockTableData();

// Get data untuk tabel keranjang
std::vector<std::vector<std::string>> getCartTableData();

// Get total belanja di keranjang
double getCartTotal();

// ============================================================
// KEUANGAN FUNCTIONS
// ============================================================

// Hitung total pemasukan
double hitungTotalPemasukan();

// Hitung total pengeluaran
double hitungTotalPengeluaran();

// Hitung laba bersih
double hitungLabaBersih();

// Get data transaksi pemasukan untuk tabel
std::vector<std::vector<std::string>> getPemasukanTableData();

// Get data transaksi pengeluaran untuk tabel
std::vector<std::vector<std::string>> getPengeluaranTableData();

// Export transaksi ke CSV
std::string exportToCSV();

// ============================================================
// EXPIRED DATE HELPERS (from ExpiredHelper.h logic)
// ============================================================

// Expired status enum for TUI color mapping
enum class TUIExpiredStatus {
    SAFE,         // No warning needed
    NEAR_EXPIRED, // Within 7 days of expiring
    EXPIRED       // Already expired
};

// Get expired status for a product
TUIExpiredStatus getTUIExpiredStatus(int expDay, int expMonth, int expYear);

// Format expired date for display
std::string formatExpiredDateTUI(int expDay, int expMonth, int expYear);

// ============================================================
// FLASH SALE HELPERS
// ============================================================

/**
 * Flash sale discount tiers based on days until expiration:
 * - <= 7 days:  11.5% discount
 * - <= 14 days: 7.5% discount
 * - <= 21 days: 14.5% discount
 * - > 21 days or expired: no discount
 */

struct FlashSaleInfo {
    double effectivePrice;   // Price after discount (hargaJual if no sale)
    double discountPercent;  // Discount as percentage (0.0 to 100.0)
    bool isOnSale;           // true if flash sale discount applies
    long long daysUntilExpired; // Days until expiration
};

// Get complete flash sale information for a product
FlashSaleInfo getFlashSaleInfo(const Database::barang& b);

// Convenience function: get effective selling price (with flash sale if applicable)
double getEffectivePrice(const Database::barang& b);

// Check if an item qualifies for flash sale
bool isFlashSaleEligible(const Database::barang& b);

// ============================================================
// EXPIRED STOCK AUTO-REMOVAL
// ============================================================

/**
 * Process all stock items and auto-remove expired items:
 * - If item is expired AND stock > 0:
 *   - Reset stock to 0
 *   - Record as "kehilangan" transaction (loss)
 *   - Add item name to return list
 * 
 * @param db Reference to Database for saving changes
 * @param dbPath Path to database.json
 * @param transaksiPath Path to transaksi.json
 * @return Vector of removed item names (for notification display)
 */
std::vector<std::string> processExpiredStock(Database& db, 
                                              const std::string& dbPath,
                                              const std::string& transaksiPath);

/**
 * Check if an item is expired (convenience wrapper)
 */
bool isItemExpired(const Database::barang& b);

}

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_HELPERS_H

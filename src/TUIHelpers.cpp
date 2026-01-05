#include "TUIHelpers.h"
#include "Database.h"
#include "Kasir.h"
#include "PathHelper.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>

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

} // namespace TUI

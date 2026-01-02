#include "../header/Keuangan.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

void clearScreen();

// ==================== FINANCIAL CALCULATION FUNCTIONS ====================

double Keuangan::hitungTotalPemasukan() {
    double total = 0;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            total += t.jumlah;
        }
    }
    return total;
}

double Keuangan::hitungTotalPengeluaran() {
    double total = 0;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pengeluaran") {
            total += t.jumlah;
        }
    }
    return total;
}

double Keuangan::hitungLabaBersih() {
    return hitungTotalPemasukan() - hitungTotalPengeluaran();
}

// ==================== DISPLAY FUNCTIONS ====================

// Konstanta untuk lebar tabel (konsisten di seluruh output)
const int TABLE_WIDTH = 70;
const int COL_NO = 4;
const int COL_TANGGAL = 12;
const int COL_WAKTU = 10;
const int COL_KETERANGAN = 28;
const int COL_JUMLAH = 14;

// Helper function untuk membuat garis separator
void printSeparator(char c = '-') {
    cout << string(TABLE_WIDTH, c) << endl;
}

// Helper function untuk membuat header terpusat
void printCenteredHeader(const string& text) {
    int padding = (TABLE_WIDTH - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

void Keuangan::tampilRingkasanKeuangan() {
    double pemasukan = hitungTotalPemasukan();
    double pengeluaran = hitungTotalPengeluaran();
    double laba = hitungLabaBersih();
    
    cout << endl;
    printSeparator('=');
    printCenteredHeader("RINGKASAN KEUANGAN");
    printSeparator('=');
    
    cout << fixed << setprecision(0);
    cout << "  " << left << setw(30) << "Total Pemasukan" 
         << ": Rp " << right << setw(15) << pemasukan << endl;
    cout << "  " << left << setw(30) << "Total Pengeluaran" 
         << ": Rp " << right << setw(15) << pengeluaran << endl;
    printSeparator('-');
    cout << "  " << left << setw(30) << "LABA BERSIH" 
         << ": Rp " << right << setw(15) << laba << endl;
    printSeparator('=');
}

void Keuangan::tampilRincianPemasukan() {
    // Filter dan copy hanya transaksi pemasukan
    vector<Database::Transaksi> pemasukan;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            pemasukan.push_back(t);
        }
    }
    
    // Sort descending by date+time (terbaru di atas)
    sort(pemasukan.begin(), pemasukan.end(),
        [](const Database::Transaksi& a, const Database::Transaksi& b) {
            // DD-MM-YYYY format, convert to YYYYMMDD for proper sorting
            string dateA = a.tanggal.substr(6,4) + a.tanggal.substr(3,2) + a.tanggal.substr(0,2) + a.waktu;
            string dateB = b.tanggal.substr(6,4) + b.tanggal.substr(3,2) + b.tanggal.substr(0,2) + b.waktu;
            return dateA > dateB;
        });
    
    cout << endl;
    printSeparator('=');
    printCenteredHeader("RINCIAN PEMASUKAN");
    printSeparator('=');
    cout << "  Total transaksi: " << pemasukan.size() << endl;
    printSeparator('-');
    
    if (pemasukan.empty()) {
        cout << "  Belum ada transaksi pemasukan." << endl;
    } else {
        // Header tabel
        cout << left 
             << " " << setw(COL_NO) << "No"
             << setw(COL_TANGGAL) << "Tanggal"
             << setw(COL_WAKTU) << "Waktu"
             << setw(COL_KETERANGAN) << "Keterangan"
             << right << setw(COL_JUMLAH) << "Jumlah (Rp)" << endl;
        printSeparator('-');
        
        // Data rows dengan nomor urut
        int no = 1;
        double total = 0;
        for (const auto& t : pemasukan) {
            // Potong keterangan jika terlalu panjang
            string ket = t.keterangan;
            if (ket.length() > COL_KETERANGAN - 2) {
                ket = ket.substr(0, COL_KETERANGAN - 5) + "...";
            }
            
            cout << left
                 << " " << setw(COL_NO) << no++
                 << setw(COL_TANGGAL) << t.tanggal
                 << setw(COL_WAKTU) << t.waktu
                 << setw(COL_KETERANGAN) << ket
                 << right << setw(COL_JUMLAH) << fixed << setprecision(0) << t.jumlah << endl;
            total += t.jumlah;
        }
        
        // Footer dengan total
        printSeparator('-');
        cout << right << setw(TABLE_WIDTH - COL_JUMLAH) << "TOTAL: "
             << setw(COL_JUMLAH) << fixed << setprecision(0) << total << endl;
    }
    
    printSeparator('=');
    cout << "\n  Tekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}

void Keuangan::tampilRincianPengeluaran() {
    // Filter dan copy hanya transaksi pengeluaran
    vector<Database::Transaksi> pengeluaran;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pengeluaran") {
            pengeluaran.push_back(t);
        }
    }
    
    // Sort descending by date+time (terbaru di atas)
    sort(pengeluaran.begin(), pengeluaran.end(),
        [](const Database::Transaksi& a, const Database::Transaksi& b) {
            // DD-MM-YYYY format, convert to YYYYMMDD for proper sorting
            string dateA = a.tanggal.substr(6,4) + a.tanggal.substr(3,2) + a.tanggal.substr(0,2) + a.waktu;
            string dateB = b.tanggal.substr(6,4) + b.tanggal.substr(3,2) + b.tanggal.substr(0,2) + b.waktu;
            return dateA > dateB;
        });
    
    cout << endl;
    printSeparator('=');
    printCenteredHeader("RINCIAN PENGELUARAN");
    printSeparator('=');
    cout << "  Total transaksi: " << pengeluaran.size() << endl;
    printSeparator('-');
    
    if (pengeluaran.empty()) {
        cout << "  Belum ada transaksi pengeluaran." << endl;
    } else {
        // Header tabel
        cout << left 
             << " " << setw(COL_NO) << "No"
             << setw(COL_TANGGAL) << "Tanggal"
             << setw(COL_WAKTU) << "Waktu"
             << setw(COL_KETERANGAN) << "Keterangan"
             << right << setw(COL_JUMLAH) << "Jumlah (Rp)" << endl;
        printSeparator('-');
        
        // Data rows dengan nomor urut
        int no = 1;
        double total = 0;
        for (const auto& t : pengeluaran) {
            // Potong keterangan jika terlalu panjang
            string ket = t.keterangan;
            if (ket.length() > COL_KETERANGAN - 2) {
                ket = ket.substr(0, COL_KETERANGAN - 5) + "...";
            }
            
            cout << left
                 << " " << setw(COL_NO) << no++
                 << setw(COL_TANGGAL) << t.tanggal
                 << setw(COL_WAKTU) << t.waktu
                 << setw(COL_KETERANGAN) << ket
                 << right << setw(COL_JUMLAH) << fixed << setprecision(0) << t.jumlah << endl;
            total += t.jumlah;
        }
        
        // Footer dengan total
        printSeparator('-');
        cout << right << setw(TABLE_WIDTH - COL_JUMLAH) << "TOTAL: "
             << setw(COL_JUMLAH) << fixed << setprecision(0) << total << endl;
    }
    
    printSeparator('=');
    cout << "\n  Tekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// ==================== EXPORT FUNCTION ====================

void Keuangan::exportToCSV() {
    // Generate filename: Laporan_Keuangan_YYYY_MM.csv
    string fileName = "Laporan_Keuangan_" + getCurrentMonthYear() + ".csv";
    string fullPath = getDocumentsPath() + "\\" + fileName;
    
    ofstream file(fullPath);
    
    if (!file.is_open()) {
        cout << "\nError: Tidak dapat membuat file CSV.\n";
        cout << "Path: " << fullPath << endl;
        return;
    }
    
    // Header CSV
    file << "ID,Tanggal,Waktu,Jenis,Keterangan,Jumlah" << endl;
    
    // Data rows
    for (const auto& t : datasetTransaksi) {
        file << t.id << ","
             << t.tanggal << ","
             << t.waktu << ","
             << t.jenis << ","
             << "\"" << t.keterangan << "\","
             << fixed << setprecision(0) << t.jumlah << endl;
    }
    
    // Summary at the end
    file << endl;
    file << "=== RINGKASAN ===" << endl;
    file << "Total Pemasukan,,,," << hitungTotalPemasukan() << endl;
    file << "Total Pengeluaran,,,," << hitungTotalPengeluaran() << endl;
    file << "Laba Bersih,,,," << hitungLabaBersih() << endl;
    
    file.close();
    
    cout << "\n========================================" << endl;
    cout << "Export berhasil!" << endl;
    cout << "File: " << fileName << endl;
    cout << "Lokasi: " << fullPath << endl;
    cout << "========================================" << endl;
    cout << "\nTekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// ==================== MENU FUNCTIONS ====================

void Keuangan::menuKeuangan() {
    cout << "\n===============================" << endl;
    cout << "SELAMAT DATANG DI MENU KEUANGAN" << endl;
    cout << "===============================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. LAPORAN KEUANGAN" << endl;
    cout << "2. EXPORT LAPORAN KEUANGAN KE CSV" << endl;
    cout << "3. KEMBALI" << endl;
    cout << "===============================" << endl;
}

void Keuangan::menuLaporan() {
    // Tampilkan ringkasan di atas menu
    tampilRingkasanKeuangan();
    
    cout << "\n===============================" << endl;
    cout << "MENU LAPORAN KEUANGAN" << endl;
    cout << "===============================" << endl;
    cout << "1. RINCIAN PENGELUARAN" << endl;
    cout << "2. RINCIAN PEMASUKAN" << endl;
    cout << "3. KEMBALI" << endl;
    cout << "===============================" << endl;
}

int Keuangan::pilihLaporan() {
    Database db;
    
    while (true) {
        clearScreen();
        
        // Load transaksi data
        db.loadTransaksi(getTransaksiPath());
        
        menuLaporan();
        cout << "Silahkan Pilih Menu (1/2/3) : "; cin >> Pilihan;
        switch (Pilihan) {
        case 1:
            clearScreen();
            tampilRincianPengeluaran();
            break;
        case 2:
            clearScreen();
            tampilRincianPemasukan();
            break;
        case 3:
            return 0; // Kembali ke pilihMenu()
        default:
            cout << "Input anda tidak valid\n\n";
            break;
        }
    }   
}

int Keuangan::pilihMenu() {
    Database db;
    
    while (true) {
        clearScreen();
        
        // Load transaksi data
        db.loadTransaksi(getTransaksiPath());
        
        menuKeuangan();
        cout << "Silahkan Pilih Menu (1/2/3) : "; cin >> Pilihan;
        switch (Pilihan) {
            case 1:
                pilihLaporan();
                break;
            case 2:
                clearScreen();
                exportToCSV();
                break;
            case 3:
                return 0; // Kembali ke main menu
            default:
                cout << "Input anda tidak valid\n\n";
                break;
        }
    }
}
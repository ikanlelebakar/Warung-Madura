/**
 * ============================================================
 * MODUL KEUANGAN - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : Keuangan.cpp
 * Deskripsi   : Modul untuk mengelola dan menampilkan laporan
 *               keuangan termasuk pemasukan, pengeluaran,
 *               dan export ke CSV
 * ============================================================
 */

#include "../header/Keuangan.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// Deklarasi fungsi eksternal dari Main.cpp
void clearScreen();

// ============================================================
// KONSTANTA UNTUK FORMAT TABEL
// ============================================================

// Lebar tabel dan kolom (konsisten di seluruh output)
const int TABLE_WIDTH = 80;
const int COL_NO = 4;
const int COL_TANGGAL = 12;
const int COL_WAKTU = 10;
const int COL_KETERANGAN = 25;
const int COL_JUMLAH = 14;
const int COL_METODE = 10;      // Kolom metode pembayaran

// Lebar menu keuangan
const int KEUANGAN_MENU_WIDTH = 40;

// ============================================================
// FUNGSI HELPER KEUANGAN
// ============================================================

/**
 * Fungsi untuk memberikan delay singkat
 * @param ms - durasi dalam milidetik
 */
void keuanganDelay(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

/**
 * Fungsi untuk mencetak garis separator tabel
 * @param c - karakter yang digunakan (default: '-')
 */
void printSeparator(char c = '-') {
    cout << string(TABLE_WIDTH, c) << endl;
}

/**
 * Fungsi untuk mencetak header terpusat dalam tabel
 * @param text - teks yang akan di-center
 */
void printCenteredHeader(const string& text) {
    int padding = (TABLE_WIDTH - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

/**
 * Fungsi untuk mencetak separator menu keuangan
 */
void printKeuanganSeparator(char c = '=') {
    cout << string(KEUANGAN_MENU_WIDTH, c) << endl;
}

/**
 * Fungsi untuk mencetak teks terpusat di menu keuangan
 */
void printKeuanganCenter(const string& text) {
    int padding = (KEUANGAN_MENU_WIDTH - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// ============================================================
// FUNGSI PERHITUNGAN KEUANGAN
// ============================================================

/**
 * Fungsi untuk menghitung total pemasukan
 * Menjumlahkan semua transaksi dengan jenis "pemasukan"
 * @return double - total pemasukan
 */
double Keuangan::hitungTotalPemasukan() {
    double total = 0;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            total += t.jumlah;
        }
    }
    return total;
}

/**
 * Fungsi untuk menghitung total pengeluaran
 * Menjumlahkan semua transaksi dengan jenis "pengeluaran"
 * @return double - total pengeluaran
 */
double Keuangan::hitungTotalPengeluaran() {
    double total = 0;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pengeluaran") {
            total += t.jumlah;
        }
    }
    return total;
}

/**
 * Fungsi untuk menghitung laba bersih
 * Laba = Total Pemasukan - Total Pengeluaran
 * @return double - laba bersih (bisa negatif jika rugi)
 */
double Keuangan::hitungLabaBersih() {
    return hitungTotalPemasukan() - hitungTotalPengeluaran();
}

// ============================================================
// FUNGSI TAMPILAN RINGKASAN KEUANGAN
// ============================================================

/**
 * Fungsi untuk menampilkan ringkasan keuangan
 * Menampilkan total pemasukan, pengeluaran, dan laba bersih
 */
void Keuangan::tampilRingkasanKeuangan() {
    double pemasukan = hitungTotalPemasukan();
    double pengeluaran = hitungTotalPengeluaran();
    double laba = hitungLabaBersih();
    
    cout << endl;
    printSeparator('=');
    printCenteredHeader("RINGKASAN KEUANGAN");
    printSeparator('=');
    
    // Tampilkan data dengan format rupiah
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

// ============================================================
// FUNGSI TAMPILAN RINCIAN PEMASUKAN
// ============================================================

/**
 * Fungsi untuk menampilkan rincian transaksi pemasukan
 * Data diurutkan berdasarkan tanggal dan waktu (terbaru di atas)
 * Menampilkan kolom metode pembayaran
 */
void Keuangan::tampilRincianPemasukan() {
    // Filter hanya transaksi pemasukan
    vector<Database::Transaksi> pemasukan;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pemasukan") {
            pemasukan.push_back(t);
        }
    }
    
    // Sort descending by date+time (terbaru di atas)
    // Konversi format DD-MM-YYYY ke YYYYMMDD untuk sorting yang benar
    sort(pemasukan.begin(), pemasukan.end(),
        [](const Database::Transaksi& a, const Database::Transaksi& b) {
            string dateA = a.tanggal.substr(6,4) + a.tanggal.substr(3,2) + a.tanggal.substr(0,2) + a.waktu;
            string dateB = b.tanggal.substr(6,4) + b.tanggal.substr(3,2) + b.tanggal.substr(0,2) + b.waktu;
            return dateA > dateB;
        });
    
    // Header tabel
    cout << endl;
    printSeparator('=');
    printCenteredHeader("RINCIAN PEMASUKAN");
    printSeparator('=');
    cout << "  Total transaksi: " << pemasukan.size() << endl;
    printSeparator('-');
    
    if (pemasukan.empty()) {
        cout << "  Belum ada transaksi pemasukan." << endl;
    } else {
        // Header kolom tabel
        cout << left 
             << " " << setw(COL_NO) << "No"
             << setw(COL_TANGGAL) << "Tanggal"
             << setw(COL_WAKTU) << "Waktu"
             << setw(COL_KETERANGAN) << "Keterangan"
             << setw(COL_METODE) << "Metode"
             << right << setw(COL_JUMLAH) << "Jumlah (Rp)" << endl;
        printSeparator('-');
        
        // Data rows
        int no = 1;
        double total = 0;
        for (const auto& t : pemasukan) {
            // Potong keterangan jika terlalu panjang
            string ket = t.keterangan;
            if (ket.length() > COL_KETERANGAN - 2) {
                ket = ket.substr(0, COL_KETERANGAN - 5) + "...";
            }
            
            // Potong metodePembayaran jika terlalu panjang
            string metode = t.metodePembayaran;
            if (metode.length() > COL_METODE - 2) {
                metode = metode.substr(0, COL_METODE - 3) + ".";
            }
            
            cout << left
                 << " " << setw(COL_NO) << no++
                 << setw(COL_TANGGAL) << t.tanggal
                 << setw(COL_WAKTU) << t.waktu
                 << setw(COL_KETERANGAN) << ket
                 << setw(COL_METODE) << metode
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

// ============================================================
// FUNGSI TAMPILAN RINCIAN PENGELUARAN
// ============================================================

/**
 * Fungsi untuk menampilkan rincian transaksi pengeluaran
 * Data diurutkan berdasarkan tanggal dan waktu (terbaru di atas)
 */
void Keuangan::tampilRincianPengeluaran() {
    // Filter hanya transaksi pengeluaran
    vector<Database::Transaksi> pengeluaran;
    for (const auto& t : datasetTransaksi) {
        if (t.jenis == "pengeluaran") {
            pengeluaran.push_back(t);
        }
    }
    
    // Sort descending by date+time (terbaru di atas)
    sort(pengeluaran.begin(), pengeluaran.end(),
        [](const Database::Transaksi& a, const Database::Transaksi& b) {
            string dateA = a.tanggal.substr(6,4) + a.tanggal.substr(3,2) + a.tanggal.substr(0,2) + a.waktu;
            string dateB = b.tanggal.substr(6,4) + b.tanggal.substr(3,2) + b.tanggal.substr(0,2) + b.waktu;
            return dateA > dateB;
        });
    
    // Header tabel
    cout << endl;
    printSeparator('=');
    printCenteredHeader("RINCIAN PENGELUARAN");
    printSeparator('=');
    cout << "  Total transaksi: " << pengeluaran.size() << endl;
    printSeparator('-');
    
    if (pengeluaran.empty()) {
        cout << "  Belum ada transaksi pengeluaran." << endl;
    } else {
        // Header kolom tabel (tanpa kolom metode untuk pengeluaran)
        cout << left 
             << " " << setw(COL_NO) << "No"
             << setw(COL_TANGGAL) << "Tanggal"
             << setw(COL_WAKTU) << "Waktu"
             << setw(COL_KETERANGAN) << "Keterangan"
             << right << setw(COL_JUMLAH) << "Jumlah (Rp)" << endl;
        printSeparator('-');
        
        // Data rows
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

// ============================================================
// FUNGSI EXPORT KE CSV
// ============================================================

/**
 * Fungsi untuk mengexport laporan keuangan ke file CSV
 * File akan disimpan di folder Documents user
 * Nama file: Laporan_Keuangan_YYYY_MM.csv
 */
void Keuangan::exportToCSV() {
    // Generate nama file dengan format: Laporan_Keuangan_YYYY_MM.csv
    string fileName = "Laporan_Keuangan_" + getCurrentMonthYear() + ".csv";
    string fullPath = getDocumentsPath() + "\\" + fileName;
    
    // Buka file untuk ditulis
    ofstream file(fullPath);
    
    if (!file.is_open()) {
        cout << "\n  [!] Error: Tidak dapat membuat file CSV." << endl;
        cout << "  Path: " << fullPath << endl;
        return;
    }
    
    // Tulis header CSV
    file << "ID,Tanggal,Waktu,Jenis,Keterangan,Jumlah,MetodePembayaran" << endl;
    
    // Tulis data transaksi
    for (const auto& t : datasetTransaksi) {
        file << t.id << ","
             << t.tanggal << ","
             << t.waktu << ","
             << t.jenis << ","
             << "\"" << t.keterangan << "\","
             << fixed << setprecision(0) << t.jumlah << ","
             << t.metodePembayaran << endl;
    }
    
    // Tulis ringkasan di bagian akhir
    file << endl;
    file << "=== RINGKASAN ===" << endl;
    file << "Total Pemasukan,,,," << hitungTotalPemasukan() << endl;
    file << "Total Pengeluaran,,,," << hitungTotalPengeluaran() << endl;
    file << "Laba Bersih,,,," << hitungLabaBersih() << endl;
    
    file.close();
    
    // Tampilkan konfirmasi
    cout << endl;
    printSeparator('=');
    printCenteredHeader("EXPORT BERHASIL!");
    printSeparator('=');
    cout << "  File   : " << fileName << endl;
    cout << "  Lokasi : " << fullPath << endl;
    printSeparator('=');
    cout << "\n  Tekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}

// ============================================================
// FUNGSI TAMPILAN MENU
// ============================================================

/**
 * Fungsi untuk menampilkan menu utama keuangan
 */
void Keuangan::menuKeuangan() {
    cout << endl;
    printKeuanganSeparator('=');
    printKeuanganCenter("MENU KEUANGAN");
    printKeuanganSeparator('=');
    printKeuanganCenter("M E N U");
    printKeuanganSeparator('-');
    cout << "  1. LAPORAN KEUANGAN" << endl;
    cout << "  2. EXPORT KE CSV" << endl;
    cout << "  3. KEMBALI KE MENU UTAMA" << endl;
    printKeuanganSeparator('=');
}

/**
 * Fungsi untuk menampilkan menu laporan keuangan
 * Menampilkan ringkasan keuangan di atas menu
 */
void Keuangan::menuLaporan() {
    // Tampilkan ringkasan terlebih dahulu
    tampilRingkasanKeuangan();
    
    cout << endl;
    printKeuanganSeparator('=');
    printKeuanganCenter("MENU LAPORAN");
    printKeuanganSeparator('=');
    cout << "  1. RINCIAN PENGELUARAN" << endl;
    cout << "  2. RINCIAN PEMASUKAN" << endl;
    cout << "  3. KEMBALI" << endl;
    printKeuanganSeparator('=');
}

// ============================================================
// FUNGSI PILIH MENU LAPORAN
// ============================================================

/**
 * Fungsi untuk menangani pilihan menu laporan
 * @return int - status (0 = kembali)
 */
int Keuangan::pilihLaporan() {
    Database db;
    
    while (true) {
        clearScreen();
        
        // Load data transaksi terbaru
        db.loadTransaksi(getTransaksiPath());
        
        menuLaporan();
        cout << "  Silahkan Pilih Menu (1/2/3): ";
        cin >> Pilihan;
        
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
                return 0; // Kembali ke menu keuangan
            default:
                cout << "\n  Input tidak valid!" << endl;
                keuanganDelay(1500);
                break;
        }
    }   
}

// ============================================================
// FUNGSI PILIH MENU KEUANGAN (UTAMA)
// ============================================================

/**
 * Fungsi utama untuk menangani menu keuangan
 * @return int - status (0 = kembali ke menu utama)
 */
int Keuangan::pilihMenu() {
    Database db;
    
    while (true) {
        clearScreen();
        
        // Load data transaksi terbaru
        db.loadTransaksi(getTransaksiPath());
        
        menuKeuangan();
        cout << "  Silahkan Pilih Menu (1/2/3): ";
        cin >> Pilihan;
        
        switch (Pilihan) {
            case 1:
                // Masuk ke menu laporan
                pilihLaporan();
                break;
            case 2:
                // Export ke CSV
                clearScreen();
                exportToCSV();
                break;
            case 3:
                // Kembali ke menu utama
                return 0;
            default:
                cout << "\n  Input tidak valid!" << endl;
                keuanganDelay(1500);
                break;
        }
    }
}
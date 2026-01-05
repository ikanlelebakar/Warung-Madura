/**
 * ============================================================
 * MODUL KASIR - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : Kasir.cpp
 * Deskripsi   : Modul untuk menangani proses transaksi penjualan
 *               termasuk keranjang belanja, checkout, dan nota
 * ============================================================
 */

#include "../header/Kasir.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// Variabel global untuk menyimpan item dalam keranjang belanja
vector<ItemBelanja> keranjang;

// Deklarasi fungsi eksternal dari Main.cpp
void clearScreen();

// Konstanta untuk lebar menu kasir
const int KASIR_MENU_WIDTH = 40;

// ============================================================
// FUNGSI HELPER KASIR
// ============================================================

/**
 * Fungsi untuk memberikan delay singkat
 * @param ms - durasi dalam milidetik
 */
void kasirDelay(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

/**
 * Fungsi untuk mencetak separator menu kasir
 */
void printKasirSeparator(char c = '=') {
    cout << string(KASIR_MENU_WIDTH, c) << endl;
}

/**
 * Fungsi untuk mencetak teks terpusat di menu kasir
 */
void printKasirCenter(const string& text) {
    int padding = (KASIR_MENU_WIDTH - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// ============================================================
// FUNGSI TAMPILAN MENU
// ============================================================

/**
 * Fungsi untuk menampilkan menu utama kasir
 * Menampilkan opsi untuk melanjutkan belanja atau kembali
 */
void Kasir::menuKasir() {
    cout << endl;
    printKasirSeparator('=');
    printKasirCenter("MENU KASIR");
    printKasirSeparator('=');
    printKasirCenter("M E N U");
    printKasirSeparator('-');
    cout << "  1. LANJUT KE DAFTAR BELANJA" << endl;
    cout << "  2. KEMBALI KE MENU UTAMA" << endl;
    printKasirSeparator('=');
}

// ============================================================
// FUNGSI KERANJANG BELANJA
// ============================================================

/**
 * Fungsi untuk menampilkan isi keranjang belanja
 * Menampilkan tabel berisi code barang, jumlah, harga, dan subtotal
 * Juga menghitung dan menampilkan total harga
 */
void Kasir::tampilKeranjang() {
    double total = 0;
    
    // Header tabel keranjang
    cout << "\n";
    cout << string(54, '=') << endl;
    cout << "           KERANJANG BELANJA" << endl;
    cout << string(54, '=') << endl;
    
    // Header kolom
    cout << left
         << setw(12) << "Nama Barang"
         << setw(12) << "Jumlah"
         << setw(15) << "Harga"
         << setw(15) << "Subtotal"
         << endl;
    
    cout << string(54, '-') << endl;
    
    // Tampilkan setiap item dalam keranjang
    for (const auto &item : keranjang) {
        double subtotal = item.harga * item.jumlah;
        total += subtotal;
        
        cout << left
             << setw(12) << item.namaBarang
             << setw(12) << item.jumlah
             << setw(15) << fixed << setprecision(0) << item.harga
             << setw(15) << subtotal
             << endl;
    }
    
    // Footer dengan total harga
    cout << string(54, '-') << endl;
    cout << right << setw(39) << "TOTAL HARGA: Rp " 
         << fixed << setprecision(0) << total << endl;
    cout << string(54, '=') << endl;
}

// ============================================================
// FUNGSI METODE PEMBAYARAN
// ============================================================

/**
 * Fungsi untuk memilih metode pembayaran
 * User dapat memilih antara CASH atau CASHLESS
 * @return string - metode yang dipilih ("CASH" atau "CASHLESS")
 */
string Kasir::menuPrintNota() {
    while (true) {
        clearScreen();
        cout << endl;
        printKasirSeparator('=');
        printKasirCenter("METODE PEMBAYARAN");
        printKasirSeparator('=');
        cout << "  1. CASH (Tunai)" << endl;
        cout << "  2. CASHLESS (Non-Tunai)" << endl;
        printKasirSeparator('=');
        cout << "  Pilih metode (1/2): ";
        cin >> pilihanMetode;
        
        switch (pilihanMetode) {
            case 1:
                return "CASH";
            case 2:
                return "CASHLESS";
            default:
                cout << "\n  Input tidak valid!" << endl;
                cout << "  Silakan pilih 1 atau 2." << endl;
                kasirDelay(1500);
                break;
        }
    }
}

// ============================================================
// FUNGSI NOTA PEMBAYARAN
// ============================================================

/**
 * Fungsi untuk mencetak nota pembayaran
 * Menampilkan ringkasan belanja, metode pembayaran, dan menyimpan transaksi
 * User dapat memilih untuk mencetak nota atau hanya menyimpan
 */
void Kasir::printNota() {
    Database database;
    
    // Hitung total harga dari semua item di keranjang
    double totalHarga = 0;
    for (const auto &item : keranjang) {
        totalHarga += item.harga * item.jumlah;
    }
    
    // Minta user memilih metode pembayaran SEBELUM menampilkan nota
    std::string metode = menuPrintNota();
    
    // Tampilkan nota pembayaran
    clearScreen();
    cout << endl;
    cout << string(54, '=') << endl;
    cout << "           NOTA PEMBAYARAN" << endl;
    cout << string(54, '=') << endl;
    
    tampilKeranjang();
    
    cout << "\n  METODE PEMBAYARAN: " << metode << endl;
    cout << string(54, '=') << endl;
    cout << "     TERIMAKASIH ATAS KUNJUNGAN ANDA!" << endl;
    cout << string(54, '=') << endl;
    
    // Tanya user apakah ingin mencetak atau hanya simpan
    cout << "\n  Pilihan (p = print nota / s = simpan saja): ";
    cin >> print;
    cin.ignore();
    
    // Simpan perubahan stok barang ke database
    database.saveToJson(getDatabasePath());
    
    // Buat dan simpan data transaksi sebagai pemasukan
    Database::Transaksi trans;
    trans.id = generateTransactionId();         // Generate ID unik
    trans.tanggal = getCurrentDate();           // Tanggal saat ini
    trans.waktu = getCurrentTime();             // Waktu saat ini
    trans.jenis = "pemasukan";                  // Jenis transaksi
    trans.keterangan = "Penjualan barang";      // Keterangan
    trans.jumlah = totalHarga;                  // Total nominal
    trans.metodePembayaran = (metode == "CASH") ? "cash" : "cashless";
    
    // Simpan transaksi ke file JSON
    database.loadTransaksi(getTransaksiPath());
    database.tambahTransaksi(trans);
    database.saveTransaksi(getTransaksiPath());
    
    // Kosongkan keranjang setelah transaksi selesai
    keranjang.clear();
    
    // Tampilkan pesan konfirmasi
    if (print == 'p' || print == 'P') {
        cout << "\n  Nota berhasil dicetak." << endl;
    }
    cout << "  Transaksi berhasil disimpan." << endl;
    cout << "\n  Tekan Enter untuk kembali...";
    cin.get();
}

// ============================================================
// FUNGSI CHECKOUT
// ============================================================

/**
 * Fungsi untuk proses checkout
 * Menampilkan keranjang dan meminta konfirmasi user
 * Jika dibatalkan, stok barang akan dikembalikan
 */
void Kasir::checkout() {
    char pilihan;
    
    clearScreen();
    cout << endl;
    cout << string(54, '=') << endl;
    cout << "              CHECKOUT" << endl;
    cout << string(54, '=') << endl;
    
    // Tampilkan isi keranjang
    tampilKeranjang();
    
    // Minta konfirmasi dari user
    cout << "\n  Konfirmasi transaksi? (y = bayar / n = batal): ";
    cin >> pilihan;
    
    if (pilihan == 'y' || pilihan == 'Y') {
        // User mengkonfirmasi, lanjut ke proses nota
        printNota();
    } else {
        // User membatalkan, kembalikan semua stok
        for (const auto &item : keranjang) {
            for (auto &barang : datasetBarang) {
                if (barang.codeBarang == item.codeBarang) {
                    barang.jumlahBarang += item.jumlah;
                    break;
                }
            }
        }
        // Kosongkan keranjang
        keranjang.clear();
        
        cout << "\n  Transaksi dibatalkan." << endl;
        cout << "  Stok barang telah dikembalikan." << endl;
        cout << "\n  Tekan Enter untuk kembali...";
        cin.ignore();
        cin.get();
    }
}

// ============================================================
// FUNGSI MENU BELANJA
// ============================================================

/**
 * Fungsi untuk proses belanja
 * User dapat menambahkan barang ke keranjang secara berulang
 * Setelah selesai, akan dilanjutkan ke proses checkout
 * @return int - status (0 = kembali tanpa belanja, 1 = selesai checkout)
 */
int Kasir::menuBelanja() {
    int codeBarang, jumlahBarang;
    char pilihan;
    Database database;
    
    // Load data barang dari database
    database.loadFromJson(getDatabasePath());
    
    cout << endl;
    cout << string(63, '=') << endl;
    cout << "            DAFTAR BARANG TERSEDIA" << endl;
    cout << string(63, '=') << endl;
    
    // Tampilkan daftar barang yang tersedia
    database.tampilBarang();
    cout << string(63, '-') << endl;

    // Loop untuk menambahkan barang ke keranjang
    do {
        bool ditemukan = false;

        cout << "\n  Input Code Barang (0 untuk kembali): ";
        cin >> codeBarang;

        // Jika user input 0, kembali ke menu sebelumnya
        if (codeBarang == 0) {
            return 0;
        }

        // Cari barang berdasarkan code
        for (auto &barang : datasetBarang) {
            if (barang.codeBarang == codeBarang) {
                ditemukan = true;

                cout << "  Masukkan jumlah barang: ";
                cin >> jumlahBarang;

                // Cek apakah stok mencukupi
                if (jumlahBarang > barang.jumlahBarang) {
                    cout << "  [!] Stok tidak mencukupi!" << endl;
                    cout << "      Stok tersedia: " << barang.jumlahBarang << endl;
                    break;
                }
                
                // Tambahkan ke keranjang dan kurangi stok
                keranjang.push_back({barang.nama, barang.codeBarang, jumlahBarang, barang.hargaBarang});
                barang.jumlahBarang -= jumlahBarang;
                cout << "  [v] Barang berhasil ditambahkan ke keranjang" << endl;
                break;
            }
        }
        
        // Jika barang tidak ditemukan
        if (!ditemukan) {
            cout << "  [!] Barang dengan code tersebut tidak ditemukan" << endl;
        }
        
        // Tanya apakah ingin tambah barang lagi
        cout << "\n  Tambah produk lagi? (y/n): ";
        cin >> pilihan;
        
    } while (pilihan == 'y' || pilihan == 'Y');

    // Lanjut ke proses checkout
    checkout();

    return 1;
}

// ============================================================
// FUNGSI PILIH MENU KASIR
// ============================================================

/**
 * Fungsi untuk menangani pilihan menu kasir
 * Menggunakan loop while agar user bisa kembali ke menu kasir
 * setelah selesai transaksi
 * @return int - status (2 = kembali ke menu utama)
 */
int Kasir::pilihMenu() {
    // Loop menu kasir
    while (true) {
        clearScreen();
        menuKasir();
        
        cout << "  Silahkan Pilih Menu (1/2): ";
        cin >> Pilihan;
        
        switch (Pilihan) {
            case 1:
                // Lanjut ke menu belanja
                clearScreen();
                menuBelanja();
                break;
                
            case 2:
                // Kembali ke menu utama
                return 2;
                
            default:
                cout << "\n  Input tidak valid!" << endl;
                cout << "  Silakan pilih 1 atau 2." << endl;
                kasirDelay(1500);
                break;
        }
    }
    
    return 0;
}
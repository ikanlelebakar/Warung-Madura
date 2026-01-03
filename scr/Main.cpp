/**
 * ============================================================
 * SISTEM PENCATATAN STOK DAN KEUANGAN WARUNG MADURA
 * ============================================================
 * File        : Main.cpp
 * Deskripsi   : File utama program yang berisi fungsi main()
 *               dan menu utama aplikasi
 * ============================================================
 */

#include "../header/Kasir.h"
#include "../header/Stock.h"
#include "../header/PathHelper.h"
#include "../header/Database.h"
#include "../header/Keuangan.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

// Deklarasi objek global untuk setiap modul
Kasir kasir;
Stock stock;
Keuangan keuangan;

// Konstanta untuk lebar menu (konsisten di seluruh program)
const int MENU_WIDTH = 40;

// ============================================================
// FUNGSI HELPER
// ============================================================

/**
 * Fungsi untuk membersihkan layar terminal
 * Menggunakan perintah sistem yang sesuai dengan OS
 */
void clearScreen() {
    #ifdef _WIN32
        system("cls");      // Windows
    #else
        system("clear");    // Linux/Mac
    #endif
}

/**
 * Fungsi untuk memberikan delay singkat saat transisi menu
 * Berguna untuk memberikan feedback visual kepada user
 * @param milliseconds - durasi delay dalam milidetik
 */
void delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

/**
 * Fungsi untuk mencetak garis separator menu
 * @param c - karakter yang digunakan untuk separator
 */
void printMenuSeparator(char c = '=') {
    cout << string(MENU_WIDTH, c) << endl;
}

/**
 * Fungsi untuk mencetak teks terpusat dalam menu
 * @param text - teks yang akan di-center
 */
void printMenuCenter(const string& text) {
    int padding = (MENU_WIDTH - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// ============================================================
// FUNGSI MENU UTAMA
// ============================================================

/**
 * Fungsi untuk menampilkan menu utama program
 * Menu ini adalah halaman pertama yang dilihat user
 */
void Menu() {
    cout << endl;
    printMenuSeparator('=');
    printMenuCenter("SELAMAT DATANG DI WARUNG MADURA");
    printMenuSeparator('=');
    printMenuCenter("M E N U");
    printMenuSeparator('-');
    cout << "  1. KASIR" << endl;
    cout << "  2. STOCK" << endl;
    cout << "  3. KEUANGAN" << endl;
    cout << "  4. KELUAR" << endl;
    printMenuSeparator('=');
}

// ============================================================
// FUNGSI MAIN (ENTRY POINT PROGRAM)
// ============================================================

/**
 * Fungsi utama program
 * Menginisialisasi database dan menjalankan loop menu utama
 */
int main() {
    // Inisialisasi objek database
    Database db;
    string dbPath = getDatabasePath();          // Path ke database.json
    string transPath = getTransaksiPath();      // Path ke transaksi.json

    // Inisialisasi file database jika belum ada
    // Ini memastikan program tidak crash jika file JSON belum dibuat
    db.initDatabase(dbPath);
    db.initTransaksi(transPath);
    
    // Load data dari file JSON ke memory
    db.loadFromJson(dbPath);
    db.loadTransaksi(transPath);

    // Variabel untuk menyimpan pilihan menu user
    int pilihan;

    // Loop utama program - terus berjalan sampai user memilih KELUAR
    do {
        clearScreen();
        Menu();
        cout << "  Silahkan Pilih Menu (1/2/3/4) : ";
        cin >> pilihan;

        // Proses pilihan user menggunakan switch-case
        switch (pilihan) {
            case 1:
                // Menu Kasir - untuk proses transaksi penjualan
                clearScreen();
                kasir.pilihMenu();
                break;
                
            case 2:
                // Menu Stock - untuk mengelola stok barang
                clearScreen();
                stock.pilihMenu();
                break;
                
            case 3:
                // Menu Keuangan - untuk melihat laporan keuangan
                clearScreen();
                keuangan.pilihMenu();
                break;
                
            case 4:
                // Keluar dari program
                clearScreen();
                printMenuSeparator('=');
                printMenuCenter("TERIMAKASIH TELAH MENGGUNAKAN");
                printMenuCenter("PROGRAM INI");
                printMenuSeparator('=');
                cout << endl;
                delay(1000);  // Delay 1 detik sebelum program berakhir
                break;
                
            default:
                // Input tidak valid
                cout << "\n  MASUKAN ANDA TIDAK VALID!" << endl;
                cout << "  Silahkan pilih angka 1-4" << endl;
                delay(1500);  // Delay agar user bisa membaca pesan error
                break;
        }
    } while (pilihan != 4);

    return 0;
}
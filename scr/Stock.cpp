/**
 * ============================================================
 * MODUL STOCK - Sistem Pencatatan Warung Madura
 * ============================================================
 * File        : Stock.cpp
 * Deskripsi   : Modul untuk mengelola stok barang, termasuk
 *               melihat daftar stok, menambah, mengubah,
 *               dan menghapus barang
 * ============================================================
 */

// === HEADER MODUL INTERNAL ===
#include "../header/Stock.h"      // Deklarasi class Stock untuk manajemen barang
#include "../header/Database.h"   // Akses datasetBarang untuk CRUD dan penyimpanan JSON
#include "../header/PathHelper.h" // Path database, validasi input, generate ID transaksi

// === STANDARD LIBRARY ===
#include <iostream>   // Input/output: cout untuk tampilan menu, cin untuk input user
#include <algorithm>  // sort() untuk sorting stok, transform() untuk toLower, replace() untuk underscore
#include <iomanip>    // Manipulator format: setw, setprecision untuk format tabel
#include <thread>     // std::this_thread::sleep_for untuk stockDelay()
#include <chrono>     // std::chrono::milliseconds untuk durasi delay

using namespace std;

// Deklarasi fungsi eksternal dari Main.cpp
void clearScreen();

// Konstanta untuk lebar menu stock
const int STOCK_MENU_WIDTH = 40;

// ============================================================
// FUNGSI HELPER STOCK
// ============================================================

/**
 * Fungsi untuk mengubah string menjadi huruf kecil semua
 * @param s - string input
 * @return string - hasil dalam huruf kecil
 */
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

/**
 * Fungsi untuk mengubah underscore menjadi spasi
 * Digunakan agar user bisa input nama barang dengan underscore
 * @param s - string input dengan underscore
 * @return string - hasil dengan spasi
 */
string underscoreToSpace(string s) {
    replace(s.begin(), s.end(), '_', ' ');
    return s;
}

/**
 * Fungsi untuk memberikan delay singkat
 * @param ms - durasi dalam milidetik
 */
void stockDelay(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

/**
 * Fungsi untuk mencetak separator menu stock
 */
void printStockSeparator(char c = '=') {
    cout << string(STOCK_MENU_WIDTH, c) << endl;
}

/**
 * Fungsi untuk mencetak teks terpusat di menu stock
 */
void printStockCenter(const string& text) {
    int padding = (STOCK_MENU_WIDTH - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// ============================================================
// FUNGSI TAMPILAN MENU
// ============================================================

/**
 * Fungsi untuk menampilkan menu utama stock
 */
void Stock::menuStock() {
    cout << endl;
    printStockSeparator('=');
    printStockCenter("MENU STOCK");
    printStockSeparator('=');
    printStockCenter("M E N U");
    printStockSeparator('-');
    cout << "  1. DAFTAR STOCK" << endl;
    cout << "  2. EDIT STOCK" << endl;
    cout << "  3. KEMBALI KE MENU UTAMA" << endl;
    printStockSeparator('=');
}

/**
 * Fungsi untuk menampilkan menu edit stock
 */
void Stock::menuEdit() {
    cout << endl;
    printStockSeparator('=');
    printStockCenter("MENU EDIT STOCK");
    printStockSeparator('=');
    printStockCenter("M E N U");
    printStockSeparator('-');
    cout << "  1. UBAH STOCK" << endl;
    cout << "  2. TAMBAH BARANG BARU" << endl;
    cout << "  3. HAPUS BARANG" << endl;
    cout << "  4. KEMBALI" << endl;
    printStockSeparator('=');
}

/**
 * Fungsi untuk menampilkan menu sort stock
 */
void Stock::menuSort() {
    cout << endl;
    printStockSeparator('=');
    printStockCenter("MENU DAFTAR STOCK");
    printStockSeparator('=');
    printStockCenter("M E N U");
    printStockSeparator('-');
    cout << "  1. URUTKAN BERDASARKAN JUMLAH" << endl;
    cout << "  2. KEMBALI" << endl;
    printStockSeparator('=');
}

// ============================================================
// FUNGSI UBAH STOCK
// ============================================================

/**
 * Fungsi untuk mengubah data stock barang yang sudah ada
 * User dapat mengubah nama, jumlah, dan harga barang
 * @return int - status (1 = berhasil, 0 = gagal, -1 = kembali)
 */
int Stock::ubahStock() {
    Database database;
    
    // Load data terbaru dari database
    database.loadFromJson(getDatabasePath());
    
    cout << endl;
    cout << string(63, '=') << endl;
    cout << "            DAFTAR BARANG" << endl;
    cout << string(63, '=') << endl;
    database.tampilBarang();
    cout << string(63, '-') << endl;

    int codeCari;
    cout << "\n  Masukkan Code Barang (0 untuk kembali): ";
    codeCari = getValidInt("");

    // Jika user input 0, kembali ke menu edit
    if (codeCari == 0) {
        cout << "\n  Kembali ke menu edit..." << endl;
        stockDelay(500);
        return -1;
    }

    bool ditemukan = false;

    // Cari barang berdasarkan code
    for (auto& barang : datasetBarang) {
        if (barang.codeBarang == codeCari) {
            ditemukan = true;

            // Tampilkan data lama
            cout << "\n  Data lama:" << endl;
            cout << "  -------------------------" << endl;
            cout << "  Nama   : " << barang.nama << endl;
            cout << "  Code   : " << barang.codeBarang << endl;
            cout << "  Jumlah : " << barang.jumlahBarang << endl;
            cout << "  Harga  : Rp " << fixed << setprecision(0) << barang.hargaBarang << endl;

            // Input data baru
            cout << "\n  Masukkan data baru:" << endl;
            cout << "  (Untuk spasi gunakan '_' sebagai pengganti)" << endl;
            cout << "  Nama Barang: ";
            cin.ignore();
            string namaInput;
            getline(cin, namaInput);
            
            // Update nama jika user mengisi
            if (!namaInput.empty()) {
                barang.nama = underscoreToSpace(toLower(namaInput));
            }

            cout << "  Jumlah Barang: "; 
            barang.jumlahBarang = getValidInt("");
            
            cout << "  Harga Barang : "; 
            barang.hargaBarang = getValidDouble("");

            // Simpan perubahan ke file JSON
            database.saveToJson(getDatabasePath());

            cout << "\n  [v] Stock berhasil diubah!" << endl;
            return 1;
        }
    }
    
    // Barang tidak ditemukan
    if (!ditemukan) {
        cout << "\n  [!] Barang dengan code tersebut tidak ditemukan" << endl;
    }
    return 0;
}

// ============================================================
// FUNGSI TAMBAH BARANG
// ============================================================

/**
 * Fungsi untuk menambahkan barang baru ke database
 * Code barang akan di-generate otomatis
 * Penambahan barang akan dicatat sebagai pengeluaran
 * @return int - status (1 = berhasil, 0 = gagal, -1 = kembali)
 */
int Stock::tambahBarang() {
    Database database;
    
    // Load data terbaru
    database.loadFromJson(getDatabasePath());

    Database::barang barangBaru;
    string namaInput;

    cout << endl;
    printStockSeparator('=');
    printStockCenter("TAMBAH BARANG BARU");
    printStockSeparator('=');
    cout << "  (Gunakan '_' untuk spasi, contoh: coca_cola)" << endl;
    printStockSeparator('-');

    // Input nama barang
    cout << "  Nama Barang (0 untuk kembali): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, namaInput);

    // Jika user input 0, kembali ke menu edit
    if (namaInput == "0") {
        cout << "\n  Kembali ke menu edit..." << endl;
        stockDelay(500);
        return -1;
    }

    // Konversi ke lowercase dan ganti underscore dengan spasi
    namaInput = toLower(namaInput);
    namaInput = underscoreToSpace(namaInput);

    // Cek apakah nama kosong
    if (namaInput.empty()) {
        cout << "\n  [!] Nama barang tidak boleh kosong!" << endl;
        return 0;
    }

    // Cek apakah nama barang sudah ada (duplikat)
    for (const auto& b : datasetBarang) {
        if (toLower(b.nama) == namaInput) {
            cout << "\n  [!] Barang dengan nama tersebut sudah ada!" << endl;
            return 0;
        }
    }

    barangBaru.nama = namaInput;

    // Input jumlah barang
    cout << "  Jumlah Barang: ";
    barangBaru.jumlahBarang = getValidInt("");

    // Input harga barang
    cout << "  Harga Barang : ";
    barangBaru.hargaBarang = getValidDouble("");

    // Generate code barang otomatis (code terakhir + 1)
    int maxCode = 999;
    for (const auto& b : datasetBarang) {
        if (b.codeBarang > maxCode) {
            maxCode = b.codeBarang;
        }
    }
    barangBaru.codeBarang = maxCode + 1;

    // Tambahkan ke dataset dan simpan
    datasetBarang.push_back(barangBaru);
    database.saveToJson(getDatabasePath());

    // Catat sebagai transaksi pengeluaran (pembelian stok)
    Database::Transaksi trans;
    trans.id = generateTransactionId();
    trans.tanggal = getCurrentDate();
    trans.waktu = getCurrentTime();
    trans.jenis = "pengeluaran";                    // Tipe: pengeluaran
    trans.keterangan = barangBaru.nama;
    trans.jumlah = barangBaru.hargaBarang * barangBaru.jumlahBarang;
    trans.metodePembayaran = "";                    // Pengeluaran tidak perlu metode
    
    // Simpan transaksi
    database.loadTransaksi(getTransaksiPath());
    database.tambahTransaksi(trans);
    database.saveTransaksi(getTransaksiPath());

    // Tampilkan konfirmasi
    cout << endl;
    printStockSeparator('-');
    cout << "  [v] Barang baru berhasil ditambahkan!" << endl;
    cout << "  Code Barang: " << barangBaru.codeBarang << endl;
    cout << "  Pengeluaran: Rp " << fixed << setprecision(0) << trans.jumlah << endl;
    printStockSeparator('=');

    return 1;
}

// ============================================================
// FUNGSI HAPUS BARANG
// ============================================================

/**
 * Fungsi untuk menghapus barang dari database
 * Setelah penghapusan, code barang akan di-renumber ulang
 * @return int - status (1 = berhasil, 0 = gagal/batal, -1 = kembali)
 */
int Stock::hapusBarang() {
    Database database;
    
    // Load data terbaru
    database.loadFromJson(getDatabasePath());
    
    cout << endl;
    cout << string(63, '=') << endl;
    cout << "            DAFTAR BARANG" << endl;
    cout << string(63, '=') << endl;
    database.tampilBarang();
    cout << string(63, '-') << endl;

    // Cek apakah ada barang untuk dihapus
    if (datasetBarang.empty()) {
        cout << "\n  [!] Tidak ada barang untuk dihapus." << endl;
        return 0;
    }

    int codeCari;
    cout << "\n  Masukkan Code Barang (0 untuk kembali): ";
    codeCari = getValidInt("");

    // Jika user input 0, kembali ke menu edit
    if (codeCari == 0) {
        cout << "\n  Kembali ke menu edit..." << endl;
        stockDelay(500);
        return -1;
    }

    bool ditemukan = false;
    int indexHapus = -1;

    // Cari barang berdasarkan code
    for (int i = 0; i < (int)datasetBarang.size(); i++) {
        if (datasetBarang[i].codeBarang == codeCari) {
            ditemukan = true;
            indexHapus = i;

            // Tampilkan data yang akan dihapus
            cout << "\n  Data yang akan dihapus:" << endl;
            cout << "  -------------------------" << endl;
            cout << "  Nama   : " << datasetBarang[i].nama << endl;
            cout << "  Code   : " << datasetBarang[i].codeBarang << endl;
            cout << "  Jumlah : " << datasetBarang[i].jumlahBarang << endl;
            cout << "  Harga  : Rp " << fixed << setprecision(0) << datasetBarang[i].hargaBarang << endl;
            break;
        }
    }

    // Jika barang tidak ditemukan
    if (!ditemukan) {
        cout << "\n  [!] Barang dengan code tersebut tidak ditemukan" << endl;
        return 0;
    }

    // Minta konfirmasi sebelum menghapus
    char konfirmasi;
    cout << "\n  Apakah anda yakin ingin menghapus? (y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        // Hapus barang dari vector
        datasetBarang.erase(datasetBarang.begin() + indexHapus);

        // Renumber code barang mulai dari 1000
        for (int i = 0; i < (int)datasetBarang.size(); i++) {
            datasetBarang[i].codeBarang = 1000 + i;
        }

        // Simpan perubahan ke file JSON
        database.saveToJson(getDatabasePath());
        
        cout << "\n  [v] Barang berhasil dihapus!" << endl;
        cout << "  Code barang telah diperbarui." << endl;
        return 1;
    } else {
        cout << "\n  Penghapusan dibatalkan." << endl;
        return 0;
    }
}

// ============================================================
// FUNGSI PILIH MENU EDIT
// ============================================================

/**
 * Fungsi untuk menangani pilihan menu edit stock
 * @return int - status (2 = kembali)
 */
int Stock::pilihEditMenu() {
    while (true) {
        clearScreen();
        menuEdit();
        cout << "  Silahkan Pilih Menu (1/2/3/4): ";
        Pilihan = getValidIntInput("", 1, 4);

        switch (Pilihan) {
            case 1: {
                // Ubah stock dengan opsi mengulang
                char pilihanKembali;
                do {
                    clearScreen();
                    int result = ubahStock();
                    // Jika user memilih kembali (-1), keluar dari loop
                    if (result == -1) break;
                    cout << "\n  Ubah data lagi? (y/n): ";
                    cin >> pilihanKembali;
                } while (pilihanKembali == 'y' || pilihanKembali == 'Y');
                break;
            }
            case 2: {
                // Tambah barang baru
                clearScreen();
                int result = tambahBarang();
                // Jika user tidak memilih kembali, tunggu input
                if (result != -1) {
                    cout << "\n  Tekan Enter untuk kembali...";
                    cin.get();
                }
                break;
            }
            case 3: {
                // Hapus barang dengan opsi mengulang
                char pilihanKembali;
                do {
                    clearScreen();
                    int result = hapusBarang();
                    // Jika user memilih kembali (-1), keluar dari loop
                    if (result == -1) break;
                    cout << "\n  Hapus barang lagi? (y/n): ";
                    cin >> pilihanKembali;
                } while (pilihanKembali == 'y' || pilihanKembali == 'Y');
                break;
            }
            case 4:
                // Kembali ke menu sebelumnya
                return 2;
                // NOTE: default case tidak diperlukan karena validasi
                // sudah dilakukan oleh getValidIntInput()
        }
    }
}

// ============================================================
// FUNGSI SORTING & TAMPIL DAFTAR STOCK
// ============================================================

/**
 * Fungsi untuk menampilkan daftar stock dengan opsi sorting
 * Data dapat diurutkan berdasarkan jumlah barang (ascending)
 * @return int - status (2 = kembali)
 */
int Stock::sortFunction() {
    Database database;
    database.loadFromJson(getDatabasePath());
    
    while (true) {
        clearScreen();
        
        cout << endl;
        cout << string(63, '=') << endl;
        cout << "            DAFTAR BARANG" << endl;
        cout << string(63, '=') << endl;
        database.tampilBarang();
        cout << string(63, '=') << endl;
        
        menuSort();
        cout << "  Silahkan Pilih Menu (1/2): ";
        Pilihan = getValidIntInput("", 1, 2);
        
        switch (Pilihan) {
            case 1: {
                // Buat salinan data untuk sorting (tidak mengubah data asli)
                vector<Database::barang> sortedData = datasetBarang;
                
                // Sort berdasarkan jumlahBarang (ascending: terkecil ke terbesar)
                sort(sortedData.begin(), sortedData.end(), 
                    [](const Database::barang& a, const Database::barang& b) {
                        return a.jumlahBarang < b.jumlahBarang;
                    });
                
                // Tampilkan hasil sorting
                clearScreen();
                cout << endl;
                cout << string(48, '=') << endl;
                cout << "  DAFTAR BARANG (DIURUTKAN BERDASARKAN JUMLAH)" << endl;
                cout << string(48, '=') << endl;
                cout << left << setw(8) << "CODE" 
                     << setw(20) << "NAMA" 
                     << setw(10) << "JUMLAH" 
                     << setw(12) << "HARGA" << endl;
                cout << string(48, '-') << endl;
                
                // Tampilkan setiap barang
                for (const auto& b : sortedData) {
                    cout << left << setw(8) << b.codeBarang
                         << setw(20) << b.nama
                         << setw(10) << b.jumlahBarang
                         << setw(12) << fixed << setprecision(0) << b.hargaBarang << endl;
                }
                
                cout << string(48, '=') << endl;
                cout << "\n  Tekan Enter untuk kembali...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2:
                // Kembali ke menu sebelumnya
                return 2;
                // NOTE: default case tidak diperlukan karena validasi
                // sudah dilakukan oleh getValidIntInput()
        }
    }
}

// ============================================================
// FUNGSI PILIH MENU STOCK (UTAMA)
// ============================================================

/**
 * Fungsi utama untuk menangani menu stock
 * @return int - status (2 = kembali ke menu utama)
 */
int Stock::pilihMenu() {
    while (true) {
        clearScreen();
        menuStock();
        cout << "  Silahkan Pilih Menu (1/2/3): ";
        Pilihan = getValidIntInput("", 1, 3);
        
        switch (Pilihan) {
            case 1:
                // Lihat daftar stock dengan opsi sorting
                clearScreen();
                sortFunction();
                break;
            case 2:
                // Masuk ke menu edit stock
                clearScreen();
                pilihEditMenu();
                break;
            case 3:
                // Kembali ke menu utama
                return 2;
                // NOTE: default case tidak diperlukan karena validasi
                // sudah dilakukan oleh getValidIntInput()
        }
    }
    return 0;
}
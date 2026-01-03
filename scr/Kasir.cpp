#include "../header/Kasir.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
vector<ItemBelanja> keranjang;

void clearScreen();

void Kasir::menuKasir() {
    cout << "\n===============================" << endl;
    cout << "SELAMAT DATANG DI MENU KASIR" << endl;
    cout << "===============================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. LANJUT KE DAFTAR BELANJA" << endl;
    cout << "2. KEMBALI" << endl;
    cout << "===============================" << endl;
}

void Kasir::printNota() {
    Database database;
    
    // Hitung total harga dari keranjang
    double totalHarga = 0;
    for (const auto &item : keranjang) {
        totalHarga += item.harga * item.jumlah;
    }
    
    // Get payment method FIRST before displaying nota
    std::string metode = menuPrintNota();
    
    clearScreen();
    cout << "\n========= NOTA PEMBAYARAN =========\n";
    tampilKeranjang();
    cout << "\nMETODE PEMBAYARAN: " << metode << endl;
    cout << "===================================\n";
    cout << "TERIMAKASIH ATAS KUNJUNGAN ANDA!\n";
    cout << "===================================\n\n";
    
    cout << "Pilihan (p = print nota / s = simpan saja): ";
    cin >> print;
    cin.ignore();
    
    // Simpan data barang
    database.saveToJson(getDatabasePath());
    
    // Simpan transaksi sebagai pemasukan
    Database::Transaksi trans;
    trans.id = generateTransactionId();
    trans.tanggal = getCurrentDate();
    trans.waktu = getCurrentTime();
    trans.jenis = "pemasukan";
    trans.keterangan = "Penjualan barang";
    trans.jumlah = totalHarga;
    trans.metodePembayaran = (metode == "CASH") ? "cash" : "cashless";
    
    database.loadTransaksi(getTransaksiPath());
    database.tambahTransaksi(trans);
    database.saveTransaksi(getTransaksiPath());
    
    keranjang.clear();
    
    if (print == 'p' || print == 'P') {
        // Print nota - tunggu Enter
        cout << "\nNota berhasil dicetak.\n";
        cout << "Transaksi berhasil dicatat. Terima kasih.\n";
        cout << "Tekan Enter untuk kembali ke menu kasir...";
        cin.get();
        clearScreen();
        pilihMenu();
    } else {
        // Simpan saja - langsung kembali
        cout << "\nTransaksi disimpan. Terima kasih.\n";
        clearScreen();
        pilihMenu();
    }
}

string Kasir::menuPrintNota() {
    while (true) {
        clearScreen();
        cout << "\n===== METODE PEMBAYARAN =====\n";
        cout << "1. CASH\n";
        cout << "2. CASHLESS\n";
        cout << "=============================\n";
        cout << "Pilih metode (1/2): ";
        cin >> pilihanMetode;
        
        switch (pilihanMetode) {
            case 1:
                return "CASH";
            case 2:
                return "CASHLESS";
            default:
                cout << "Input tidak valid, silakan pilih 1 atau 2.\n";
                break;
        }
    }
}

void Kasir::tampilKeranjang() {
    double total = 0;
    
    cout << "\n===== KERANJANG BELANJA =====\n";
    cout << left
         << setw(12) << "Code"
         << setw(12) << "Jumlah"
         << setw(15) << "Harga"
         << setw(15) << "Subtotal"
         << endl;
    
    cout << string(54, '-') << endl;
    for (const auto &item : keranjang) {
        double subtotal = item.harga * item.jumlah;
        total += subtotal;
        
        cout << left
             << setw(12) << item.codeBarang
             << setw(12) << item.jumlah
             << setw(15) << item.harga
             << setw(15) << subtotal
             << endl;
    }
    cout << string(54, '-') << endl;
    cout << right << setw(39) << "TOTAL HARGA: Rp " << fixed << setprecision(0) << total << endl;
}

void Kasir::checkout() {
    char pilihan;
    
    clearScreen();
    
    cout << "\n===== CHECKOUT =====\n";
    tampilKeranjang();
    
    cout << "\nKonfirmasi transaksi? (y = bayar / n = batal): ";
    cin >> pilihan;
    
    if (pilihan == 'y' || pilihan == 'Y') {
        // Lanjut ke print nota
        printNota();
    } else {
        // Batalkan transaksi - kembalikan stok
        for (const auto &item : keranjang) {
            for (auto &barang : datasetBarang) {
                if (barang.codeBarang == item.codeBarang) {
                    barang.jumlahBarang += item.jumlah;
                    break;
                }
            }
        }
        keranjang.clear();
        cout << "\nTransaksi dibatalkan. Stok dikembalikan.\n";
        cout << "Tekan Enter untuk kembali...";
        cin.ignore();
        cin.get();

        pilihMenu();
    }
}

int Kasir::menuBelanja() {
    int codeBarang, jumlahBarang;
    char pilihan;
    Database database;
    database.loadFromJson(getDatabasePath());
    database.tampilBarang();

    do {
        bool ditemukan = false;

        cout << "Input Code Barang (0 untuk kembali) : ";
        cin >> codeBarang;

        if (codeBarang == 0) {
            return false;
        }

        for (auto &barang : datasetBarang) {
            if (barang.codeBarang == codeBarang) {
                ditemukan = true;

                cout << "Masukkan jumlah barang : ";
                cin >> jumlahBarang;

                if (jumlahBarang > barang.jumlahBarang) {
                    cout << "Stock tidak mencukupi\n";
                    break;
                }
                keranjang.push_back({codeBarang, jumlahBarang, barang.hargaBarang});
                barang.jumlahBarang -= jumlahBarang;
                cout << "Barang ditambahkan ke keranjang\n";
                break;
            }
        }
        if (!ditemukan) {
            cout << "Barang tidak ada di database\n";
        }
        cout << "Apakah ingin menambahkan produk lagi (y/n)? ";
        cin >> pilihan;
    } while (pilihan == 'y' || pilihan == 'Y');

    checkout();

    return 1;
}

int Kasir::pilihMenu() {
    menuKasir();
    cout << "Silahkan Pilih Menu (1/2) : "; cin >> Pilihan;
    switch (Pilihan) {
        case 1:
            clearScreen();
            menuBelanja();
            return 0;
        case 2:
            return 2;
        default:
            cout << "Input anda tidak valid\n\n";
            break;
    }
    return 1;
}
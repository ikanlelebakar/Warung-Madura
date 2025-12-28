#include "Kasir.h"
#include "Database.h"
#include "PathHelper.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;
vector<ItemBelanja> keranjang;

void Kasir::menuKasir() {
    cout << "\n===============================" << endl;
    cout << "SELAMAT DATANG DI MENU KASIR" << endl;
    cout << "===============================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. LANJUT KE DAFTAR BELANJA" << endl;
    cout << "2. KEMBALI" << endl;
    cout << "===============================" << endl;
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
    cout << right << setw(30) << "TOTAL HARGA: " << total << endl;
}

void Kasir::checkout() {
    char pilihan;
    Database database;

    cout << "\n===== CHECKOUT =====\n";
    tampilKeranjang();

    cout << "\nKonfirmasi transaksi? (y = bayar / n = batal): ";
    cin >> pilihan;

    if (pilihan == 'y' || pilihan == 'Y') {
        database.saveToJson(getDatabasePath());
        cout << "Transaksi berhasil. Terima kasih.\n";
        keranjang.clear(); // transaksi selesai
        return;
    }
    for (const auto &item : keranjang) {
        for (auto &barang : datasetBarang) {
            if (barang.codeBarang == item.codeBarang) {
                barang.jumlahBarang += item.jumlah;
                break;
            }
        }
    }
    keranjang.clear();
    cout << "Transaksi dibatalkan. Stok dikembalikan.\n";
}

int Kasir::pilihMenu() {
    cout << "Silahkan Pilih Menu (1/2) : "; cin >> Pilihan;
    switch (Pilihan) {
        case 1:
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
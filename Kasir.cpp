//
// Created by faishal on 12/17/25.
//

#include "Kasir.h"
#include <iostream>
#include <vector>

#include "Database.h"
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

    do {
        bool ditemukan = false;

        cout << "Input Code Barang : ";
        cin >> codeBarang;

        for (auto &barang : datasetBarang) {
            if (barang.codeBarang == codeBarang) {
                ditemukan = true;

                cout << "Masukkan jumlah barang : ";
                cin >> jumlahBarang;

                if (jumlahBarang > barang.jumlahBarang) {
                    cout << "Stock tidak mencukupi\n";
                    break;
                }
                keranjang.push_back({codeBarang, jumlahBarang});
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
    for (const auto &item : keranjang) {
        cout << "Code Barang: " << item.codeBarang << " | Jumlah: " << item.jumlah << endl;
    }
}

void Kasir::checkout() {
    char pilihan;

    cout << "\n===== CHECKOUT =====\n";
    tampilKeranjang();

    cout << "\nKonfirmasi transaksi? (y = bayar / n = batal): ";
    cin >> pilihan;

    if (pilihan == 'y' || pilihan == 'Y') {
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

// int main() {
//     menuBelanja();
// }
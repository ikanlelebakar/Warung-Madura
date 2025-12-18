//
// Created by faishal on 12/17/25.
//

#include "Kasir.h"
#include <iostream>
#include <vector>

#include "Database.h"
using namespace std;

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

    cout << "Input Code Barang : ";
    cin >> codeBarang;

    for (auto &barang : datasetBarang) {
        if (barang.codeBarang == codeBarang) {
            cout << "Masukkan jumlah barang : "; cin >> jumlahBarang;
            if (barang.jumlahBarang < jumlahBarang) {
                cout << "Stock tidak mencukupi" << endl;
                return 0;
            }
        }
        cout << "Stock cukup" << endl;
        return 1;
    }

    cout << "\nBarang tidak ada di database\n";
    return 1;
}

int Kasir::pilihMenu() {
    cout << "Silahkan Pilih Menu (1/2) : "; cin >> Pilihan;
    switch (Pilihan) {
        case 1:
            menuBelanja();
            return 0;
        case 2:
            return 1;
        default:
            cout << "Input anda tidak valid\n\n";
            break;
    }
    return 1;
}

// int main() {
//     menuBelanja();
// }
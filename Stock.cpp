#include "Stock.h"
#include "Database.h"
#include <iostream>
Database db;
using namespace std;

void Stock::menuStock() {
    cout << "\n================================" << endl;
    cout << "SELAMAT DATANG DI MENU STOCK" << endl;
    cout << "================================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. LANJUT KE DAFTAR STOCK" << endl;
    cout << "2. EDIT STOCK" << endl;
    cout << "3. KEMBALI" << endl;
    cout << "================================" << endl;
}

void Stock::menuEdit() {
    cout << "\n================================" << endl;
    cout << "SELAMAT DATANG DI MENU EDIT STOCK" << endl;
    cout << "================================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. UBAH STOCK" << endl;
    cout << "2. TAMBAH BARANG" << endl;
    cout << "3. KEMBALI" << endl;
    cout << "================================" << endl;
}

int Stock::ubahStock() {

}

int Stock::tambahBarang() {

}

int Stock::pilihEditMenu() {
    cout << "Silahkan Pilih Menu (1/2/3) : "; cin >> Pilihan;
    switch (Pilihan) {
        case 1:
            db.tampilBarang();
            return 1;
        case 2:
            db.tampilBarang();
            return 0;
        case 3:
            return 2;
        default:
            cout << "Input anda tidak valid\n\n";
            break;
    }
    return 1;
}

int Stock::kembali() {
    while (true) {
        cout << "Apakah Anda Ingin Kembali Ke Halaman Awal (y/n) ? "; cin >> pilihanKembali;
        if (pilihanKembali == 'y' || pilihanKembali == 'Y') {
            return 2;
        } else {
            return 1;
        }
    }
}

int Stock::pilihMenu() {
    cout << "Silahkan Pilih Menu (1/2/3) : "; cin >> Pilihan;
    switch (Pilihan) {
        case 1:
            db.tampilBarang();
            return kembali();
        case 2:
            menuEdit();
            pilihEditMenu();

            return 0;
        case 3:
            return 2;
        default:
            cout << "Input anda tidak valid\n\n";
            break;
    }
    return 1;
}
#include "../header/Stock.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Stock::menuStock() {
    cout << "\n================================" << endl;
    cout << "SELAMAT DATANG DI MENU STOCK" << endl;
    cout << "================================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. DAFTAR STOCK" << endl;
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
    Database database;
    database.loadFromJson(getDatabasePath());
    database.tampilBarang();

    int codeCari;
    cout << "\nMasukkan Code Barang yang ingin diubah: ";
    cin >> codeCari;

    bool ditemukan = false;

    for (auto& barang : datasetBarang) {
        if (barang.codeBarang == codeCari) {
            ditemukan = true;

            cout << "\nData lama:\n";
            cout << "Nama   : " << barang.nama << endl;
            cout << "Code   : " << barang.codeBarang << endl;
            cout << "Jumlah : " << barang.jumlahBarang << endl;
            cout << "Harga  : " << barang.hargaBarang << endl;

            cout << "\nMasukkan data baru\n";
            cout << "Nama Barang (untuk spasi gunakan '_' debagai pengganti)  : ";
            cin.ignore();
            getline(cin, barang.nama);

            cout << "Jumlah Barang : "; cin >> barang.jumlahBarang;
            cout << "Harga Barang  : "; cin >> barang.hargaBarang;

            database.saveToJson(getDatabasePath());

            cout << "\nStock berhasil diubah\n";
            return 1;
        }
    }
    if (!ditemukan) {
        cout << "\nBarang tidak ditemukan\n";
    }
    return 0;
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string underscoreToSpace(string s) {
    replace(s.begin(), s.end(), '_', ' ');
    return s;
}

int Stock::tambahBarang() {
    Database database;
    database.loadFromJson(getDatabasePath());

    Database::barang barangBaru;
    string namaInput;

    cout << "\nMasukkan Data Barang Baru\n";
    cout << "Gunakan '_' untuk spasi (contoh: coca_cola)\n";

    cout << "Nama Barang   : ";
    cin.ignore();
    getline(cin, namaInput);

    namaInput = toLower(namaInput);
    namaInput = underscoreToSpace(namaInput);

    // cek nama duplikat
    for (const auto& b : datasetBarang) {
        if (toLower(b.nama) == namaInput) {
            cout << "\nBarang dengan nama tersebut sudah ada.\n";
            return 0;
        }
    }

    barangBaru.nama = namaInput;

    cout << "Jumlah Barang : ";
    cin >> barangBaru.jumlahBarang;

    cout << "Harga Barang  : ";
    cin >> barangBaru.hargaBarang;

    int maxCode = 999;
    for (const auto& b : datasetBarang) {
        if (b.codeBarang > maxCode) {
            maxCode = b.codeBarang;
        }
    }
    barangBaru.codeBarang = maxCode + 1;

    datasetBarang.push_back(barangBaru);

    database.saveToJson(getDatabasePath());

    cout << "\nBarang baru berhasil ditambahkan\n";
    cout << "Code Barang Otomatis: " << barangBaru.codeBarang << endl;

    return 1;
}


int Stock::pilihEditMenu() {
    while (true) {
        menuEdit();
        cout << "Silahkan Pilih Menu (1/2/3) : ";
        cin >> Pilihan;

        switch (Pilihan) {
            case 1: {
                char pilihanKembali;
                do {
                    ubahStock();
                    cout << "Apakah anda ingin menambahkannya lagi (y/n)? ";
                    cin >> pilihanKembali;
                } while (pilihanKembali == 'y' || pilihanKembali == 'Y');
                break;
            }
            case 2:
                tambahBarang();
                break;
            case 3:
                return 2;
            default:
                cout << "Input anda tidak valid\n\n";
                break;
        }
    }
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
    Database database;

    cout << "Silahkan Pilih Menu (1/2/3) : "; cin >> Pilihan;
    switch (Pilihan) {
        case 1:
            database.loadFromJson(getDatabasePath());
            database.tampilBarang();
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
    return 0;
}
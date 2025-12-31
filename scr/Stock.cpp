#include "../header/Stock.h"
#include "../header/Database.h"
#include "../header/PathHelper.h"
#include <iostream>
#include <algorithm>

using namespace std;

void clearScreen();

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string underscoreToSpace(string s) {
    replace(s.begin(), s.end(), '_', ' ');
    return s;
}

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
    cout << "3. HAPUS BARANG" << endl;
    cout << "4. KEMBALI" << endl;
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

            cout << "\nMasukkan data baru (kosongkan untuk tidak mengubah)\n";
            cout << "Nama Barang (untuk spasi gunakan '_' debagai pengganti)  : ";
            cin.ignore();
            string namaInput;
            getline(cin, namaInput);
            if (!namaInput.empty()) {
                barang.nama = underscoreToSpace(toLower(namaInput));
            }

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

int Stock::hapusBarang() {
    Database database;
    database.loadFromJson(getDatabasePath());
    database.tampilBarang();

    if (datasetBarang.empty()) {
        cout << "\nTidak ada barang untuk dihapus.\n";
        return 0;
    }

    int codeCari;
    cout << "\nMasukkan Code Barang yang ingin dihapus: ";
    cin >> codeCari;

    bool ditemukan = false;
    int indexHapus = -1;

    for (int i = 0; i < datasetBarang.size(); i++) {
        if (datasetBarang[i].codeBarang == codeCari) {
            ditemukan = true;
            indexHapus = i;

            cout << "\nData yang akan dihapus:\n";
            cout << "Nama   : " << datasetBarang[i].nama << endl;
            cout << "Code   : " << datasetBarang[i].codeBarang << endl;
            cout << "Jumlah : " << datasetBarang[i].jumlahBarang << endl;
            cout << "Harga  : " << datasetBarang[i].hargaBarang << endl;
            break;
        }
    }

    if (!ditemukan) {
        cout << "\nBarang tidak ditemukan\n";
        return 0;
    }

    char konfirmasi;
    cout << "\nApakah anda yakin ingin menghapus barang ini? (y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        datasetBarang.erase(datasetBarang.begin() + indexHapus);

        // Renumber kode barang mulai dari 1000
        for (int i = 0; i < datasetBarang.size(); i++) {
            datasetBarang[i].codeBarang = 1000 + i;
        }

        database.saveToJson(getDatabasePath());
        cout << "\nBarang berhasil dihapus dan kode barang telah diperbarui.\n";
        return 1;
    } else {
        cout << "\nPenghapusan dibatalkan.\n";
        return 0;
    }
}

int Stock::pilihEditMenu() {
    while (true) {
        menuEdit();
        cout << "Silahkan Pilih Menu (1/2/3/4) : ";
        cin >> Pilihan;

        switch (Pilihan) {
            case 1: {
                char pilihanKembali;
                do {
                    ubahStock();
                    cout << "Apakah anda ingin mengubahnya lagi (y/n)? ";
                    cin >> pilihanKembali;
                } while (pilihanKembali == 'y' || pilihanKembali == 'Y');
                break;
            }
            case 2:
                tambahBarang();
                break;
            case 3: {
                char pilihanKembali;
                do {
                    hapusBarang();
                    cout << "Apakah anda ingin menghapus lagi (y/n)? ";
                    cin >> pilihanKembali;
                } while (pilihanKembali == 'y' || pilihanKembali == 'Y');
                break;
            }
            case 4:
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
            break;
        }
    }
    return 0;
}

int Stock::pilihMenu() {
    Database database;

    while (true) {
        menuStock();
        cout << "Silahkan Pilih Menu (1/2/3) : "; cin >> Pilihan;
        switch (Pilihan) {
            case 1:
                database.loadFromJson(getDatabasePath());
                clearScreen();
                database.tampilBarang();
                return kembali();
            case 2:
                clearScreen();
                if (pilihEditMenu() == 2) {
                    clearScreen();
                }
                break;
            case 3:
                return 2;
            default:
                cout << "Input anda tidak valid\n\n";
                clearScreen();
                break;
        }
    }
    return 0;
}
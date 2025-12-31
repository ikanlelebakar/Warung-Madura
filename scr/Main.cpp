#include "../header/Kasir.h"
#include "../header/Stock.h"
#include "../header/PathHelper.h"
#include "../header/Database.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Kasir kasir;
Stock stock;

// Function untuk membersihkan layar terminal
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Menu() {
    cout << "===============================" << endl;
    cout << "SELAMAT DATANG DI WARUNG MADURA" << endl;
    cout << "===============================" << endl;
    cout << "\t\tMENU" << endl;
    cout << "1. KASIR" << endl;
    cout << "2. STOCK" << endl;
    cout << "3. KEUANGAN" << endl;
    cout << "4. KELUAR" << endl;
    cout << "===============================" << endl;
}

int main() {
    Database db;
    string dbPath = getDatabasePath();

    db.initDatabase(dbPath);
    db.loadFromJson(dbPath);

    do {
        Menu();
        cout << "Silahkan Pilih Menu (1/2/3/4) : "; cin >> kasir.Pilihan;
        switch (kasir.Pilihan) {
            case 1:
                clearScreen();
                kasir.menuKasir();
                if (kasir.pilihMenu() == 2) {
                    clearScreen();
                    break;
                }
                clearScreen();
                break;
            case 2:
                clearScreen();
                stock.pilihMenu();
                clearScreen();
                break;
            case 3:
                clearScreen();
                break;
            case 4:
                clearScreen();
                cout << "Terimakasih Telah Menggunakan Program Ini" << endl;
                break;
            default:
                clearScreen();
                cout << "MASUKANA ANDA TIDAK VALID, SILAHKAN MENGULANG\n\n";
                break;
        }
    } while (kasir.Pilihan != 4);
    cin.ignore();

    return 0;
}
#include <iostream>
#include "Kasir.h"
using namespace std;
Kasir kasir;

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
    int Pilihan;

    do {
        Menu();
        cout << "Silahkan Pilih Menu (1/2/3/4) : "; cin >> kasir.Pilihan;
        switch (kasir.Pilihan) {
            case 1:
                kasir.menuKasir();
                if (kasir.pilihMenu() == 2) {
                    break;
                }
                kasir.menuKasir();
                kasir.pilihMenu();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                cout << "Terimakasih Telah Menggunakan Program Ini" << endl;
                break;
            default:
                cout << "MASUKANA ANDA TIDAK VALID, SILAHKAN MENGULANG\n\n";
                break;
        }
    } while (kasir.Pilihan != 4);
    cin.ignore();

    return 0;
}
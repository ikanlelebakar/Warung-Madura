#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Barang {
    string nama;
    int stock;
    double harga;
};

int main() {
    Barang b;
    vector<Barang> barang;

    cout << "Masukkan 0 apabila ingin berhenti" << endl;
    while (true) {
        cout << "Masukkan nama barang : "; getline(cin, b.nama);
        if (b.nama == "0") {
            break;
        }
        cout << "Masukkan stock barang : "; cin >> b.stock;
        cout << "Masukkan harga barang : "; cin >> b.harga;
        cin.ignore();

        barang.push_back(b);
    }

    for (size_t i = 0; i < barang.size(); i++) {
        cout << i+1 << ". "
        << "nama barang = " << barang[i].nama << " | "
        << "stock barang = " << barang[i].stock << " | "
        << "harga barang = " << barang[i].harga << " | " << endl;
    }
}
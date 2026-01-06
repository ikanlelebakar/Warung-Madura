#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H

// === STANDARD LIBRARY ===
#include <vector>  // std::vector untuk extern keranjang (daftar item belanja)
#include <string>  // std::string untuk namaBarang dalam struct ItemBelanja

struct ItemBelanja {
    std::string namaBarang;
    int codeBarang, jumlah;
    double harga;
};

class Kasir {
    public:
        void menuKasir(), checkout(), printNota();
        int pilihMenu(), Pilihan;
        
    private:
        std::string menuPrintNota();
        int menuBelanja(), pilihanMetode;
        void tampilKeranjang();
        char print;
};

extern std::vector<ItemBelanja> keranjang;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#include <vector>
#include <string>

struct ItemBelanja {
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
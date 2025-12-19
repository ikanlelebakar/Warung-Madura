//
// Created by faishal on 12/17/25.
//

#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#include <vector>

struct ItemBelanja {
    int codeBarang, jumlah;
};

class Kasir {
    public:
        void menuKasir(), checkout();;
        int pilihMenu(), Pilihan;

    private:
        int menuBelanja();
        void tampilKeranjang();
};

extern std::vector<ItemBelanja> keranjang;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
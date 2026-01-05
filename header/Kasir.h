#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#include <vector>
#include <string>

// Struct untuk item di keranjang belanja
struct ItemBelanja {
    std::string namaBarang;
    int codeBarang, jumlah;
    double harga;
};

// Global keranjang belanja
extern std::vector<ItemBelanja> keranjang;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
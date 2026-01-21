#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H

// === STANDARD LIBRARY ===
#include <vector>  // std::vector untuk extern keranjang (daftar item belanja)
#include <string>  // std::string untuk namaBarang dalam struct ItemBelanja

// Struct untuk item di keranjang belanja
struct ItemBelanja {
    std::string namaBarang;
    int codeBarang, jumlah;
    double harga;
};

// Global keranjang belanja (defined in TUI.cpp)
extern std::vector<ItemBelanja> keranjang;

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_KASIR_H
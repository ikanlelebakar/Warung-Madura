#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_HELPERS_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_HELPERS_H

#include <string>
#include <vector>

namespace TUI {

// Format angka ke format Rupiah
std::string formatRupiah(double value);

// Get data untuk tabel stock
std::vector<std::vector<std::string>> getStockTableData();

// Get data untuk tabel keranjang
std::vector<std::vector<std::string>> getCartTableData();

// Get total belanja di keranjang
double getCartTotal();

}

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_HELPERS_H

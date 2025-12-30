#include "TUIHelpers.h"
#include "Database.h"
#include "Kasir.h"
#include <sstream>
#include <iomanip>

namespace TUI {

std::string formatRupiah(double value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << value;
    return "Rp " + ss.str();
}

std::vector<std::vector<std::string>> getStockTableData() {
    std::vector<std::vector<std::string>> data;
    for (const auto& b : datasetBarang) {
        data.push_back({
            b.nama, 
            std::to_string(b.codeBarang), 
            std::to_string(b.jumlahBarang), 
            formatRupiah(b.hargaBarang)
        });
    }
    return data;
}

std::vector<std::vector<std::string>> getCartTableData() {
    std::vector<std::vector<std::string>> data;
    for (const auto& item : keranjang) {
        std::string nama = "?";
        for (const auto& b : datasetBarang) {
            if (b.codeBarang == item.codeBarang) {
                nama = b.nama;
                break;
            }
        }
        double subtotal = item.harga * item.jumlah;
        data.push_back({
            nama,
            std::to_string(item.codeBarang),
            std::to_string(item.jumlah),
            formatRupiah(item.harga),
            formatRupiah(subtotal)
        });
    }
    return data;
}

double getCartTotal() {
    double total = 0;
    for (const auto& item : keranjang) {
        total += item.harga * item.jumlah;
    }
    return total;
}

} // namespace TUI

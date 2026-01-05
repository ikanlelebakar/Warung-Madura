#include "TUIViews.h"
#include "Kasir.h"
#include "Database.h"

using namespace ftxui;

namespace TUI {

Element RenderMainMenu(Component& menu) {
    return StyledBox(vbox({
        text("") | size(HEIGHT, EQUAL, 1),
        text("Selamat Datang di Sistem Manajemen Toko") | center | bold | color(Colors::primary()),
        text("") | size(HEIGHT, EQUAL, 1),
        separator(),
        text("") | size(HEIGHT, EQUAL, 1),
        text("Pilih menu dengan Arrow Keys + Enter") | center | dim,
        text("") | size(HEIGHT, EQUAL, 1),
        menu->Render() | center,
        text("") | size(HEIGHT, EQUAL, 1),
    }), "MENU UTAMA") | size(WIDTH, GREATER_THAN, 50) | center;
}

Element RenderKasirMenu(Component& menu, const std::vector<std::string>& stockHeaders, const std::vector<std::string>& cartHeaders) {
    return hbox({
        StyledBox(menu->Render(), "Menu Kasir") | size(WIDTH, EQUAL, 25),
        vbox({
            StyledBox(StyledStockTable(stockHeaders), "Daftar Barang") | frame | flex,
            keranjang.empty() ? StyledBox(text("Keranjang kosong") | center | dim, "Keranjang") :
            StyledBox(StyledTable(getCartTableData(), cartHeaders), "Keranjang") | frame
        }) | flex
    });
}

Element RenderKasirBelanja(Component& codeInput, Component& jumlahInput, const std::vector<std::string>& stockHeaders, const std::vector<std::string>& cartHeaders) {
    return hbox({
        StyledBox(vbox({
            hbox({text("Kode   : ") | size(WIDTH, EQUAL, 9), codeInput->Render() | flex | borderRounded}),
            hbox({text("Jumlah : ") | size(WIDTH, EQUAL, 9), jumlahInput->Render() | flex | borderRounded}),
            separator(), text("[Enter] Tambah | [Esc] Selesai") | center | dim
        }), "Input Belanja") | size(WIDTH, EQUAL, 40),
        vbox({
            StyledBox(StyledStockTable(stockHeaders), "Daftar Barang") | frame | flex,
            keranjang.empty() ? StyledBox(text("Keranjang kosong") | center | dim, "Keranjang") :
            StyledBox(StyledTable(getCartTableData(), cartHeaders), "Keranjang") | frame
        }) | flex
    });
}

Element RenderCheckout(const std::vector<std::string>& cartHeaders) {
    return StyledBox(vbox({
        StyledTable(getCartTableData(), cartHeaders), separator(),
        hbox({text("TOTAL: ") | bold, text(formatRupiah(getCartTotal())) | bold | color(Colors::success())}) | center,
        separator(), text("Tekan [Y] Bayar | [N] Batal") | center | color(Colors::accent())
    }), "CHECKOUT") | center;
}

Element RenderStockMenu(Component& menu, const std::vector<std::string>& stockHeaders) {
    return hbox({
        StyledBox(menu->Render(), "Menu Stock") | size(WIDTH, EQUAL, 25),
        StyledBox(StyledStockTable(stockHeaders), "Daftar Stock") | frame | flex
    });
}

Element RenderStockAdd(Component& namaInput, Component& jumlahInput, Component& hargaInput, const std::vector<std::string>& stockHeaders) {
    return hbox({
        StyledBox(vbox({
            hbox({text("Nama   : ") | size(WIDTH, EQUAL, 9), namaInput->Render() | flex | borderRounded}),
            hbox({text("Jumlah : ") | size(WIDTH, EQUAL, 9), jumlahInput->Render() | flex | borderRounded}),
            hbox({text("Harga  : ") | size(WIDTH, EQUAL, 9), hargaInput->Render() | flex | borderRounded}),
            separator(), text("[Enter] Simpan | [Esc] Batal") | center | dim
        }), "Tambah Barang Baru") | size(WIDTH, EQUAL, 45),
        StyledBox(StyledStockTable(stockHeaders), "Stock Saat Ini") | frame | flex
    });
}

Element RenderStockEdit(Component& codeInput, Component& namaInput, Component& jumlahInput, Component& hargaInput, const std::vector<std::string>& stockHeaders) {
    return hbox({
        StyledBox(vbox({
            hbox({text("Kode   : ") | size(WIDTH, EQUAL, 9), codeInput->Render() | flex | borderRounded}),
            separator(), text("Data baru:") | dim,
            hbox({text("Nama   : ") | size(WIDTH, EQUAL, 9), namaInput->Render() | flex | borderRounded}),
            hbox({text("Jumlah : ") | size(WIDTH, EQUAL, 9), jumlahInput->Render() | flex | borderRounded}),
            hbox({text("Harga  : ") | size(WIDTH, EQUAL, 9), hargaInput->Render() | flex | borderRounded}),
            separator(), text("[Enter] Update | [Esc] Batal") | center | dim
        }), "Edit Stock") | size(WIDTH, EQUAL, 50),
        StyledBox(StyledStockTable(stockHeaders), "Stock Saat Ini") | frame | flex
    });
}

Element RenderStockDelete(Component& codeInput, const std::vector<std::string>& stockHeaders) {
    return hbox({
        StyledBox(vbox({
            hbox({text("Kode   : ") | size(WIDTH, EQUAL, 9), codeInput->Render() | flex | borderRounded}),
            separator(), text("[Enter] Cari | [Esc] Batal") | center | dim
        }), "Hapus Barang") | size(WIDTH, EQUAL, 45),
        StyledBox(StyledStockTable(stockHeaders), "Stock Saat Ini") | frame | flex
    });
}

Element RenderDeleteConfirm(const std::string& itemName, int itemCode) {
    return StyledBox(vbox({
        text("Apakah Anda yakin ingin menghapus barang ini?") | center | bold, separator(),
        hbox({text("Nama  : ") | bold, text(itemName)}) | center,
        hbox({text("Kode  : ") | bold, text(std::to_string(itemCode))}) | center,
        separator(), text("Tekan [Y] untuk Hapus | [N] untuk Batal") | center | color(Colors::danger())
    }), "KONFIRMASI HAPUS") | center | color(Colors::danger());
}

// ============================================================
// KEUANGAN VIEWS
// ============================================================

Element RenderKeuanganMenu(Component& menu) {
    // Ringkasan keuangan di sebelah kiri, menu di sebelah kanan
    double pemasukan = hitungTotalPemasukan();
    double pengeluaran = hitungTotalPengeluaran();
    double laba = hitungLabaBersih();
    
    auto ringkasanBox = StyledBox(vbox({
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({text("Total Pemasukan   : ") | bold, text(formatRupiah(pemasukan)) | color(Colors::success())}) | center,
        hbox({text("Total Pengeluaran : ") | bold, text(formatRupiah(pengeluaran)) | color(Colors::danger())}) | center,
        separator(),
        hbox({text("LABA BERSIH       : ") | bold, text(formatRupiah(laba)) | bold | color(laba >= 0 ? Colors::success() : Colors::danger())}) | center,
        text("") | size(HEIGHT, EQUAL, 1),
    }), "RINGKASAN KEUANGAN");
    
    return hbox({
        StyledBox(menu->Render(), "Menu Keuangan") | size(WIDTH, EQUAL, 30),
        ringkasanBox | flex
    });
}

Element RenderKeuanganRingkasan() {
    double pemasukan = hitungTotalPemasukan();
    double pengeluaran = hitungTotalPengeluaran();
    double laba = hitungLabaBersih();
    
    return StyledBox(vbox({
        text("") | size(HEIGHT, EQUAL, 1),
        text("RINGKASAN KEUANGAN") | bold | center | color(Colors::primary()),
        text("") | size(HEIGHT, EQUAL, 1),
        separator(),
        text("") | size(HEIGHT, EQUAL, 1),
        hbox({text("Total Pemasukan   : "), text(formatRupiah(pemasukan)) | color(Colors::success())}) | center,
        hbox({text("Total Pengeluaran : "), text(formatRupiah(pengeluaran)) | color(Colors::danger())}) | center,
        separator(),
        hbox({text("LABA BERSIH       : ") | bold, text(formatRupiah(laba)) | bold | color(laba >= 0 ? Colors::success() : Colors::danger())}) | center,
        text("") | size(HEIGHT, EQUAL, 1),
        separator(),
        text("[Esc] Kembali") | center | dim,
        text("") | size(HEIGHT, EQUAL, 1),
    }), "LAPORAN KEUANGAN") | center;
}

Element RenderKeuanganPemasukan(const std::vector<std::string>& headers) {
    auto data = getPemasukanTableData();
    
    if (data.empty()) {
        return StyledBox(vbox({
            text("Belum ada transaksi pemasukan") | center | dim,
            separator(),
            text("[Esc] Kembali") | center | dim
        }), "RINCIAN PEMASUKAN") | center;
    }
    
    double total = hitungTotalPemasukan();
    
    return StyledBox(vbox({
        text("Total transaksi: " + std::to_string(data.size())) | dim,
        separator(),
        StyledTable(data, headers) | frame | flex,
        separator(),
        hbox({text("TOTAL: ") | bold, text(formatRupiah(total)) | bold | color(Colors::success())}) | center,
        separator(),
        text("[Esc] Kembali") | center | dim
    }), "RINCIAN PEMASUKAN") | flex;
}

Element RenderKeuanganPengeluaran(const std::vector<std::string>& headers) {
    auto data = getPengeluaranTableData();
    
    if (data.empty()) {
        return StyledBox(vbox({
            text("Belum ada transaksi pengeluaran") | center | dim,
            separator(),
            text("[Esc] Kembali") | center | dim
        }), "RINCIAN PENGELUARAN") | center;
    }
    
    double total = hitungTotalPengeluaran();
    
    return StyledBox(vbox({
        text("Total transaksi: " + std::to_string(data.size())) | dim,
        separator(),
        StyledTable(data, headers) | frame | flex,
        separator(),
        hbox({text("TOTAL: ") | bold, text(formatRupiah(total)) | bold | color(Colors::danger())}) | center,
        separator(),
        text("[Esc] Kembali") | center | dim
    }), "RINCIAN PENGELUARAN") | flex;
}

Element RenderKeuanganExportResult(const std::string& filePath) {
    if (filePath.empty()) {
        return StyledBox(vbox({
            text("GAGAL!") | bold | center | color(Colors::danger()),
            separator(),
            text("Tidak dapat membuat file CSV") | center,
            separator(),
            text("[Esc] Kembali") | center | dim
        }), "EXPORT CSV") | center;
    }
    
    return StyledBox(vbox({
        text("BERHASIL!") | bold | center | color(Colors::success()),
        separator(),
        text("File CSV berhasil dibuat:") | center,
        text(filePath) | center | dim,
        separator(),
        text("[Esc] Kembali") | center | dim
    }), "EXPORT CSV") | center;
}

} // namespace TUI

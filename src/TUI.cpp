#include "TUI.h"
#include "TUIComponents.h"
#include "TUIViews.h"
#include "Database.h"
#include "PathHelper.h"
#include "Kasir.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <algorithm>

using namespace ftxui;

namespace TUI {

void Run() {
    Database db;
    std::string dbPath = getDatabasePath();
    std::string transaksiPath = getTransaksiPath();
    db.initDatabase(dbPath);
    db.initTransaksi(transaksiPath);
    db.loadFromJson(dbPath);
    db.loadTransaksi(transaksiPath);
    
    auto screen = ScreenInteractive::Fullscreen();
    
    // View states:
    // 0 = Main Menu
    // 1 = Kasir Menu, 2 = Kasir Belanja, 3 = Checkout
    // 4 = Stock Menu, 5 = Stock Add, 6 = Stock Edit, 8 = Stock Delete, 9 = Delete Confirm
    // 7 = Keuangan Menu, 10 = Keuangan Ringkasan, 11 = Pemasukan, 12 = Pengeluaran, 13 = Export Result
    int viewState = 0;
    int mainMenuSelected = 0, kasirMenuSelected = 0, stockMenuSelected = 0, keuanganMenuSelected = 0;
    std::string statusMsg = "Selamat datang! Gunakan Arrow Keys dan Enter.";
    std::string exportPath = "";
    
    std::string kasirCode, kasirJumlah, stockNama, stockJumlah, stockHarga;
    std::string editCode, editNama, editJumlah, editHarga, deleteCode, deleteItemName;
    int deleteItemCode = 0;
    
    std::vector<std::string> mainMenuEntries = {"1. KASIR", "2. STOCK", "3. KEUANGAN", "4. KELUAR"};
    std::vector<std::string> kasirMenuEntries = {"1. Mulai Belanja", "2. Checkout", "3. Kembali"};
    std::vector<std::string> stockMenuEntries = {"1. Tambah Barang", "2. Edit Stock", "3. Hapus Barang", "4. Kembali"};
    std::vector<std::string> keuanganMenuEntries = {"1. Ringkasan Keuangan", "2. Rincian Pemasukan", "3. Rincian Pengeluaran", "4. Export ke CSV", "5. Kembali"};
    std::vector<std::string> stockHeaders = {"Nama", "Kode", "Stock", "Harga"};
    std::vector<std::string> cartHeaders = {"Nama", "Kode", "Qty", "Harga", "Subtotal"};
    std::vector<std::string> pemasukanHeaders = {"Tanggal", "Waktu", "Keterangan", "Metode", "Jumlah"};
    std::vector<std::string> pengeluaranHeaders = {"Tanggal", "Waktu", "Keterangan", "Jumlah"};
    
    auto mainMenu = Menu(&mainMenuEntries, &mainMenuSelected);
    auto kasirMenu = Menu(&kasirMenuEntries, &kasirMenuSelected);
    auto stockMenu = Menu(&stockMenuEntries, &stockMenuSelected);
    auto keuanganMenu = Menu(&keuanganMenuEntries, &keuanganMenuSelected);
    
    auto kasirCodeInput = Input(&kasirCode, "Kode barang");
    auto kasirJumlahInput = Input(&kasirJumlah, "Jumlah");
    auto stockNamaInput = Input(&stockNama, "Nama barang");
    auto stockJumlahInput = Input(&stockJumlah, "Jumlah");
    auto stockHargaInput = Input(&stockHarga, "Harga");
    auto editCodeInput = Input(&editCode, "Kode barang");
    auto editNamaInput = Input(&editNama, "Nama baru");
    auto editJumlahInput = Input(&editJumlah, "Jumlah baru");
    auto editHargaInput = Input(&editHarga, "Harga baru");
    auto deleteCodeInput = Input(&deleteCode, "Kode barang");
    
    auto container = Container::Tab({
        mainMenu,                                                           // 0
        kasirMenu,                                                          // 1
        Container::Vertical({kasirCodeInput, kasirJumlahInput}),           // 2
        Renderer([]{return text("");}),                                     // 3
        stockMenu,                                                          // 4
        Container::Vertical({stockNamaInput, stockJumlahInput, stockHargaInput}), // 5
        Container::Vertical({editCodeInput, editNamaInput, editJumlahInput, editHargaInput}), // 6
        keuanganMenu,                                                       // 7
        Container::Vertical({deleteCodeInput}),                             // 8
        Renderer([]{return text("");}),                                     // 9
        Renderer([]{return text("");}),                                     // 10
        Renderer([]{return text("");}),                                     // 11
        Renderer([]{return text("");}),                                     // 12
        Renderer([]{return text("");}),                                     // 13
    }, &viewState);
    
    auto component = Renderer(container, [&]() {
        Elements content;
        content.push_back(StyledHeader("SISTEM MANAJEMEN TOKO"));
        
        switch (viewState) {
            case 0: content.push_back(filler()); content.push_back(RenderMainMenu(mainMenu)); content.push_back(filler()); break;
            case 1: content.push_back(RenderKasirMenu(kasirMenu, stockHeaders, cartHeaders)); break;
            case 2: content.push_back(RenderKasirBelanja(kasirCodeInput, kasirJumlahInput, stockHeaders, cartHeaders)); break;
            case 3: if (!keranjang.empty()) { content.push_back(filler()); content.push_back(RenderCheckout(cartHeaders)); content.push_back(filler()); } break;
            case 4: content.push_back(RenderStockMenu(stockMenu, stockHeaders)); break;
            case 5: content.push_back(RenderStockAdd(stockNamaInput, stockJumlahInput, stockHargaInput, stockHeaders)); break;
            case 6: content.push_back(RenderStockEdit(editCodeInput, editNamaInput, editJumlahInput, editHargaInput, stockHeaders)); break;
            case 7: content.push_back(RenderKeuanganMenu(keuanganMenu)); break;
            case 8: content.push_back(RenderStockDelete(deleteCodeInput, stockHeaders)); break;
            case 9: content.push_back(filler()); content.push_back(RenderDeleteConfirm(deleteItemName, deleteItemCode)); content.push_back(filler()); break;
            case 10: content.push_back(filler()); content.push_back(RenderKeuanganRingkasan()); content.push_back(filler()); break;
            case 11: content.push_back(RenderKeuanganPemasukan(pemasukanHeaders)); break;
            case 12: content.push_back(RenderKeuanganPengeluaran(pengeluaranHeaders)); break;
            case 13: content.push_back(filler()); content.push_back(RenderKeuanganExportResult(exportPath)); content.push_back(filler()); break;
        }
        content.push_back(StatusBar(statusMsg));
        return vbox(content) | flex;
    });
    
    component = CatchEvent(component, [&](Event event) {
        if (event == Event::Escape) {
            if (viewState == 1 || viewState == 4 || viewState == 7) { viewState = 0; statusMsg = "Kembali ke menu utama"; return true; }
            if (viewState == 2 || viewState == 3) { viewState = 1; statusMsg = "Kembali ke menu kasir"; return true; }
            if (viewState == 5) { viewState = 4; stockNama = stockJumlah = stockHarga = ""; statusMsg = "Kembali ke menu stock"; return true; }
            if (viewState == 6) { viewState = 4; editCode = editNama = editJumlah = editHarga = ""; statusMsg = "Kembali ke menu stock"; return true; }
            if (viewState == 8 || viewState == 9) { viewState = 4; deleteCode = deleteItemName = ""; deleteItemCode = 0; statusMsg = "Kembali ke menu stock"; return true; }
            if (viewState == 10 || viewState == 11 || viewState == 12 || viewState == 13) { viewState = 7; exportPath = ""; statusMsg = "Kembali ke menu keuangan"; return true; }
        }
        if (viewState == 3) {
            if (event == Event::Character('y') || event == Event::Character('Y')) {
                // Simpan transaksi pemasukan
                double total = getCartTotal();
                Database::Transaksi trans;
                trans.id = generateTransactionId();
                trans.tanggal = getCurrentDate();
                trans.waktu = getCurrentTime();
                trans.jenis = "pemasukan";
                trans.keterangan = "Penjualan barang";
                trans.jumlah = total;
                trans.metodePembayaran = "cash";
                db.tambahTransaksi(trans);
                db.saveTransaksi(transaksiPath);
                db.saveToJson(dbPath);
                keranjang.clear();
                viewState = 1;
                statusMsg = "Transaksi berhasil!";
                return true;
            }
            if (event == Event::Character('n') || event == Event::Character('N')) {
                for (const auto& item : keranjang) for (auto& b : datasetBarang) if (b.codeBarang == item.codeBarang) { b.jumlahBarang += item.jumlah; break; }
                keranjang.clear(); viewState = 1; statusMsg = "Transaksi dibatalkan."; return true;
            }
            return false;
        }
        if (viewState == 9) {
            if (event == Event::Character('y') || event == Event::Character('Y')) {
                auto it = std::remove_if(datasetBarang.begin(), datasetBarang.end(), [&](const Database::barang& b) { return b.codeBarang == deleteItemCode; });
                if (it != datasetBarang.end()) { datasetBarang.erase(it, datasetBarang.end()); db.reindexCodes(); db.saveToJson(dbPath); statusMsg = "Barang dihapus!"; }
                deleteCode = deleteItemName = ""; deleteItemCode = 0; viewState = 4; return true;
            }
            if (event == Event::Character('n') || event == Event::Character('N')) { deleteCode = deleteItemName = ""; deleteItemCode = 0; viewState = 4; statusMsg = "Dibatalkan"; return true; }
            return false;
        }
        if (event == Event::Return) {
            if (viewState == 0) {
                if (mainMenuSelected == 0) viewState = 1; else if (mainMenuSelected == 1) viewState = 4;
                else if (mainMenuSelected == 2) { viewState = 7; db.loadTransaksi(transaksiPath); }
                else if (mainMenuSelected == 3) screen.Exit();
                statusMsg = mainMenuSelected == 0 ? "Menu Kasir" : mainMenuSelected == 1 ? "Menu Stock" : "Menu Keuangan"; return true;
            }
            if (viewState == 1) {
                if (kasirMenuSelected == 0) { viewState = 2; statusMsg = "Ketik kode dan jumlah"; }
                else if (kasirMenuSelected == 1) { if (keranjang.empty()) statusMsg = "Keranjang kosong!"; else { viewState = 3; statusMsg = "Y=Bayar, N=Batal"; } }
                else if (kasirMenuSelected == 2) { viewState = 0; statusMsg = "Kembali"; }
                return true;
            }
            if (viewState == 2 && !kasirCode.empty() && !kasirJumlah.empty()) {
                try {
                    int code = std::stoi(kasirCode), jumlah = std::stoi(kasirJumlah); bool found = false;
                    for (auto& b : datasetBarang) if (b.codeBarang == code) { found = true;
                        if (jumlah > b.jumlahBarang) statusMsg = "Stock tidak cukup!";
                        else { keranjang.push_back({b.nama, code, jumlah, b.hargaBarang}); b.jumlahBarang -= jumlah; statusMsg = b.nama + " ditambahkan!"; kasirCode = kasirJumlah = ""; }
                        break; }
                    if (!found) statusMsg = "Tidak ditemukan!";
                } catch (...) { statusMsg = "Input tidak valid!"; }
                return true;
            }
            if (viewState == 4) {
                if (stockMenuSelected == 0) viewState = 5; else if (stockMenuSelected == 1) viewState = 6;
                else if (stockMenuSelected == 2) viewState = 8; else if (stockMenuSelected == 3) viewState = 0;
                statusMsg = stockMenuSelected == 3 ? "Kembali" : "Masukkan data"; return true;
            }
            if (viewState == 5 && !stockNama.empty() && !stockJumlah.empty() && !stockHarga.empty()) {
                try {
                    Database::barang n; n.nama = stockNama; n.jumlahBarang = std::stoi(stockJumlah); n.hargaBarang = std::stod(stockHarga);
                    int maxCode = 999; for (const auto& b : datasetBarang) if (b.codeBarang > maxCode) maxCode = b.codeBarang;
                    n.codeBarang = maxCode + 1; datasetBarang.push_back(n); db.saveToJson(dbPath);
                    statusMsg = "Ditambahkan! Kode: " + std::to_string(n.codeBarang); viewState = 4; stockNama = stockJumlah = stockHarga = "";
                } catch (...) { statusMsg = "Input tidak valid!"; }
                return true;
            }
            if (viewState == 6 && !editCode.empty()) {
                try {
                    int code = std::stoi(editCode); bool found = false;
                    for (auto& b : datasetBarang) if (b.codeBarang == code) { found = true;
                        if (!editNama.empty()) b.nama = editNama; if (!editJumlah.empty()) b.jumlahBarang = std::stoi(editJumlah);
                        if (!editHarga.empty()) b.hargaBarang = std::stod(editHarga);
                        db.saveToJson(dbPath); statusMsg = "Berhasil diupdate!"; viewState = 4; editCode = editNama = editJumlah = editHarga = ""; break; }
                    if (!found) statusMsg = "Tidak ditemukan!";
                } catch (...) { statusMsg = "Input tidak valid!"; }
                return true;
            }
            if (viewState == 7) {
                db.loadTransaksi(transaksiPath); // Reload transaksi setiap kali masuk submenu
                if (keuanganMenuSelected == 0) { viewState = 10; statusMsg = "Ringkasan Keuangan"; }
                else if (keuanganMenuSelected == 1) { viewState = 11; statusMsg = "Rincian Pemasukan"; }
                else if (keuanganMenuSelected == 2) { viewState = 12; statusMsg = "Rincian Pengeluaran"; }
                else if (keuanganMenuSelected == 3) { exportPath = exportToCSV(); viewState = 13; statusMsg = exportPath.empty() ? "Export gagal!" : "Export berhasil!"; }
                else if (keuanganMenuSelected == 4) { viewState = 0; statusMsg = "Kembali"; }
                return true;
            }
            if (viewState == 8 && !deleteCode.empty()) {
                try {
                    int code = std::stoi(deleteCode); bool found = false;
                    for (const auto& b : datasetBarang) if (b.codeBarang == code) { found = true; deleteItemName = b.nama; deleteItemCode = code; viewState = 9; statusMsg = "Y=Hapus, N=Batal"; break; }
                    if (!found) statusMsg = "Tidak ditemukan!";
                } catch (...) { statusMsg = "Input tidak valid!"; }
                return true;
            }
            if (viewState == 10 || viewState == 11 || viewState == 12 || viewState == 13) {
                viewState = 7; exportPath = ""; statusMsg = "Kembali"; return true;
            }
        }
        return false;
    });
    screen.Loop(component);
}

} // namespace TUI

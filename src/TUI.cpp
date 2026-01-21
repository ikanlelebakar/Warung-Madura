#include "TUI.h"
#include "TUIComponents.h"
#include "TUIViews.h"
#include "TUIHelpers.h"
#include "Database.h"
#include "PathHelper.h"
#include "Kasir.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <algorithm>

using namespace ftxui;

// Global keranjang belanja (moved from deleted Kasir.cpp)
std::vector<ItemBelanja> keranjang;

namespace TUI {

void Run() {
    Database db;
    std::string dbPath = getDatabasePath();
    std::string transaksiPath = getTransaksiPath();
    db.initDatabase(dbPath);
    db.initTransaksi(transaksiPath);
    db.loadFromJson(dbPath);
    db.loadTransaksi(transaksiPath);
    
    // Auto-remove expired stock at startup
    std::vector<std::string> expiredRemoved = processExpiredStock(db, dbPath, transaksiPath);
    
    auto screen = ScreenInteractive::Fullscreen();
    
    // View states:
    // 0 = Main Menu
    // 1 = Kasir Menu, 2 = Kasir Belanja, 3 = Checkout
    // 4 = Stock Menu, 5 = Stock Add, 6 = Stock Edit, 8 = Stock Delete, 9 = Delete Confirm
    // 7 = Keuangan Menu, 10 = Keuangan Ringkasan, 11 = Pemasukan, 12 = Pengeluaran, 13 = Export Result
    int viewState = 0;
    int mainMenuSelected = 0, kasirMenuSelected = 0, stockMenuSelected = 0, keuanganMenuSelected = 0;
    // Build startup message with expired notifications if any
    std::string statusMsg;
    if (!expiredRemoved.empty()) {
        statusMsg = "Expired removed: ";
        for (size_t i = 0; i < expiredRemoved.size(); ++i) {
            if (i > 0) statusMsg += ", ";
            statusMsg += expiredRemoved[i];
        }
    } else {
        statusMsg = "Selamat datang! Gunakan Arrow Keys dan Enter.";
    }
    std::string exportPath = "";
    
    // Payment method state (from main branch)
    std::string paymentMethod = "CASH";
    
    std::string kasirCode, kasirJumlah, stockNama, stockJumlah, stockHarga;
    std::string editCode, editNama, editJumlah, editHarga, deleteCode, deleteItemName;
    
    // Expired date inputs (from main branch)
    std::string stockExpDay = "0", stockExpMonth = "0", stockExpYear = "0";
    std::string editExpDay, editExpMonth, editExpYear;
    
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
    auto stockHargaInput = Input(&stockHarga, "Harga beli");
    
    // Expired date inputs for Stock Add
    auto stockExpDayInput = Input(&stockExpDay, "0");
    auto stockExpMonthInput = Input(&stockExpMonth, "0");
    auto stockExpYearInput = Input(&stockExpYear, "0");
    
    auto editCodeInput = Input(&editCode, "Kode barang");
    auto editNamaInput = Input(&editNama, "Nama baru");
    auto editJumlahInput = Input(&editJumlah, "Jumlah baru");
    auto editHargaInput = Input(&editHarga, "Harga baru");
    
    // Expired date inputs for Stock Edit
    auto editExpDayInput = Input(&editExpDay, "Hari");
    auto editExpMonthInput = Input(&editExpMonth, "Bulan");
    auto editExpYearInput = Input(&editExpYear, "Tahun");
    
    auto deleteCodeInput = Input(&deleteCode, "Kode barang");
    
    auto container = Container::Tab({
        mainMenu,                                                           // 0
        kasirMenu,                                                          // 1
        Container::Vertical({kasirCodeInput, kasirJumlahInput}),           // 2
        Renderer([]{return text("");}),                                     // 3
        stockMenu,                                                          // 4
        Container::Vertical({stockNamaInput, stockJumlahInput, stockHargaInput, 
                            stockExpDayInput, stockExpMonthInput, stockExpYearInput}), // 5
        Container::Vertical({editCodeInput, editNamaInput, editJumlahInput, editHargaInput,
                            editExpDayInput, editExpMonthInput, editExpYearInput}), // 6
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
            case 3: if (!keranjang.empty()) { content.push_back(filler()); content.push_back(RenderCheckout(cartHeaders, paymentMethod)); content.push_back(filler()); } break;
            case 4: content.push_back(RenderStockMenu(stockMenu, stockHeaders)); break;
            case 5: content.push_back(RenderStockAdd(stockNamaInput, stockJumlahInput, stockHargaInput, 
                                                     stockExpDayInput, stockExpMonthInput, stockExpYearInput, stockHeaders)); break;
            case 6: content.push_back(RenderStockEdit(editCodeInput, editNamaInput, editJumlahInput, editHargaInput,
                                                      editExpDayInput, editExpMonthInput, editExpYearInput, stockHeaders)); break;
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
            if (viewState == 2 || viewState == 3) { viewState = 1; paymentMethod = "CASH"; statusMsg = "Kembali ke menu kasir"; return true; }
            if (viewState == 5) { viewState = 4; stockNama = stockJumlah = stockHarga = ""; stockExpDay = stockExpMonth = stockExpYear = "0"; statusMsg = "Kembali ke menu stock"; return true; }
            if (viewState == 6) { viewState = 4; editCode = editNama = editJumlah = editHarga = editExpDay = editExpMonth = editExpYear = ""; statusMsg = "Kembali ke menu stock"; return true; }
            if (viewState == 8 || viewState == 9) { viewState = 4; deleteCode = deleteItemName = ""; deleteItemCode = 0; statusMsg = "Kembali ke menu stock"; return true; }
            if (viewState == 10 || viewState == 11 || viewState == 12 || viewState == 13) { viewState = 7; exportPath = ""; statusMsg = "Kembali ke menu keuangan"; return true; }
        }
        
        // Payment method toggle in Checkout (from main branch)
        if (viewState == 3) {
            if (event == Event::Character('1')) {
                paymentMethod = "CASH";
                statusMsg = "Metode: CASH";
                return true;
            }
            if (event == Event::Character('2')) {
                paymentMethod = "CASHLESS";
                statusMsg = "Metode: CASHLESS";
                return true;
            }
            if (event == Event::Character('y') || event == Event::Character('Y')) {
                // Simpan transaksi pemasukan dengan harga flash sale jika berlaku
                double total = 0;
                for (const auto& item : keranjang) {
                    // Find the item in datasetBarang to get effective price
                    for (const auto& b : datasetBarang) {
                        if (b.codeBarang == item.codeBarang) {
                            // Use getEffectivePrice for flash sale support
                            total += TUI::getEffectivePrice(b) * item.jumlah;
                            break;
                        }
                    }
                }
                
                Database::Transaksi trans;
                trans.id = generateTransactionId();
                trans.tanggal = getCurrentDate();
                trans.waktu = getCurrentTime();
                trans.jenis = "pemasukan";
                trans.keterangan = "Penjualan barang";
                trans.jumlah = total;
                trans.metodePembayaran = (paymentMethod == "CASH") ? "cash" : "cashless";
                db.tambahTransaksi(trans);
                db.saveTransaksi(transaksiPath);
                db.saveToJson(dbPath);
                keranjang.clear();
                viewState = 1;
                paymentMethod = "CASH";
                statusMsg = "Transaksi berhasil! Total: Rp " + std::to_string(static_cast<int>(total));
                return true;
            }
            if (event == Event::Character('n') || event == Event::Character('N')) {
                for (const auto& item : keranjang) for (auto& b : datasetBarang) if (b.codeBarang == item.codeBarang) { b.jumlahBarang += item.jumlah; break; }
                keranjang.clear(); viewState = 1; paymentMethod = "CASH"; statusMsg = "Transaksi dibatalkan."; return true;
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
                else if (kasirMenuSelected == 1) { if (keranjang.empty()) statusMsg = "Keranjang kosong!"; else { viewState = 3; statusMsg = "[1] CASH [2] CASHLESS | Y=Bayar N=Batal"; } }
                else if (kasirMenuSelected == 2) { viewState = 0; statusMsg = "Kembali"; }
                return true;
            }
            if (viewState == 2 && !kasirCode.empty() && !kasirJumlah.empty()) {
                try {
                    int code = std::stoi(kasirCode), jumlah = std::stoi(kasirJumlah); bool found = false;
                    for (auto& b : datasetBarang) if (b.codeBarang == code) { found = true;
                        // Block expired items from being sold
                        if (TUI::isItemExpired(b)) {
                            statusMsg = b.nama + " sudah expired! Tidak dapat dijual.";
                        }
                        else if (jumlah > b.jumlahBarang) statusMsg = "Stock tidak cukup!";
                        else { 
                            // Use effective price for cart (supports flash sale)
                            double effectivePrice = TUI::getEffectivePrice(b);
                            keranjang.push_back({b.nama, code, jumlah, effectivePrice}); 
                            b.jumlahBarang -= jumlah; 
                            
                            // Show flash sale indicator if applicable
                            if (TUI::isFlashSaleEligible(b)) {
                                statusMsg = b.nama + " ditambahkan [SALE]!";
                            } else {
                                statusMsg = b.nama + " ditambahkan!";
                            }
                            kasirCode = kasirJumlah = ""; 
                        }
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
                    Database::barang n; 
                    n.nama = stockNama; 
                    n.jumlahBarang = std::stoi(stockJumlah); 
                    n.hargaBarang = std::stod(stockHarga);
                    
                    // Auto-calculate hargaJual using markup (from main branch)
                    n.hargaJual = hitungHargaJual(n.hargaBarang);
                    
                    // Parse expired date (from main branch)
                    n.expDay = stockExpDay.empty() ? 0 : std::stoi(stockExpDay);
                    n.expMonth = stockExpMonth.empty() ? 0 : std::stoi(stockExpMonth);
                    n.expYear = stockExpYear.empty() ? 0 : std::stoi(stockExpYear);
                    
                    int maxCode = 999; for (const auto& b : datasetBarang) if (b.codeBarang > maxCode) maxCode = b.codeBarang;
                    n.codeBarang = maxCode + 1; 
                    datasetBarang.push_back(n); 
                    db.saveToJson(dbPath);
                    
                    // Record as pengeluaran (from main branch)
                    double totalPengeluaran = n.hargaBarang * n.jumlahBarang;
                    Database::Transaksi trans;
                    trans.id = generateTransactionId();
                    trans.tanggal = getCurrentDate();
                    trans.waktu = getCurrentTime();
                    trans.jenis = "pengeluaran";
                    trans.keterangan = n.nama;
                    trans.jumlah = totalPengeluaran;
                    trans.metodePembayaran = "";
                    db.loadTransaksi(transaksiPath);
                    db.tambahTransaksi(trans);
                    db.saveTransaksi(transaksiPath);
                    
                    statusMsg = "Ditambahkan! Kode: " + std::to_string(n.codeBarang) + " | H.Jual: Rp " + std::to_string(static_cast<int>(n.hargaJual)); 
                    viewState = 4; 
                    stockNama = stockJumlah = stockHarga = ""; 
                    stockExpDay = stockExpMonth = stockExpYear = "0";
                } catch (...) { statusMsg = "Input tidak valid!"; }
                return true;
            }
            if (viewState == 6 && !editCode.empty()) {
                try {
                    int code = std::stoi(editCode); bool found = false;
                    for (auto& b : datasetBarang) if (b.codeBarang == code) { found = true;
                        if (!editNama.empty()) b.nama = editNama; 
                        if (!editJumlah.empty()) b.jumlahBarang = std::stoi(editJumlah);
                        if (!editHarga.empty()) {
                            b.hargaBarang = std::stod(editHarga);
                            // Recalculate hargaJual (from main branch)
                            b.hargaJual = hitungHargaJual(b.hargaBarang);
                        }
                        
                        // Update expired date if provided (from main branch)
                        if (!editExpDay.empty()) b.expDay = std::stoi(editExpDay);
                        if (!editExpMonth.empty()) b.expMonth = std::stoi(editExpMonth);
                        if (!editExpYear.empty()) b.expYear = std::stoi(editExpYear);
                        
                        db.saveToJson(dbPath); 
                        statusMsg = "Berhasil diupdate!"; 
                        viewState = 4; 
                        editCode = editNama = editJumlah = editHarga = editExpDay = editExpMonth = editExpYear = ""; 
                        break; 
                    }
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

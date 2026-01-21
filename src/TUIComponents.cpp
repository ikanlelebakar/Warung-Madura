#include "TUIComponents.h"
#include "TUIHelpers.h"
#include "Database.h"
#include <ftxui/dom/table.hpp>

using namespace ftxui;

namespace TUI {

Element Logo() {
    return vbox({
        text("╦ ╦╔═╗╦═╗╦ ╦╔╗╔╔═╗  ╔╦╗╔═╗╔╦╗╦ ╦╦═╗╔═╗") | bold,
        text("║║║╠═╣╠╦╝║ ║║║║║ ╦  ║║║╠═╣ ║║║ ║╠╦╝╠═╣") | bold,
        text("╚╩╝╩ ╩╩╚═╚═╝╝╚╝╚═╝  ╩ ╩╩ ╩═╩╝╚═╝╩╚═╩ ╩") | bold,
    }) | color(Colors::primary()) | center;
}

Element StyledHeader(const std::string& title) {
    return vbox({
        Logo(),
        separator() | color(Colors::secondary()),
        text(title) | bold | center | color(Colors::accent()),
        separator() | color(Colors::secondary()),
    }) | borderDouble | color(Colors::primary());
}

Element StyledBox(Element content, const std::string& title) {
    if (title.empty()) {
        return content | borderRounded | color(Colors::secondary());
    }
    return window(text(" " + title + " ") | bold | color(Colors::accent()), content) 
           | color(Colors::secondary());
}

Element StyledTable(const std::vector<std::vector<std::string>>& data, 
                    const std::vector<std::string>& headers) {
    if (data.empty() && headers.empty()) {
        return text("Tidak ada data") | center | dim;
    }
    
    std::vector<std::vector<Element>> tableData;
    
    std::vector<Element> headerRow;
    for (const auto& h : headers) {
        headerRow.push_back(text(" " + h + " ") | bold | color(Colors::accent()));
    }
    tableData.push_back(headerRow);
    
    for (const auto& row : data) {
        std::vector<Element> tableRow;
        for (const auto& cell : row) {
            tableRow.push_back(text(" " + cell + " "));
        }
        tableData.push_back(tableRow);
    }
    
    auto table = Table(tableData);
    
    table.SelectAll().Border(LIGHT);
    table.SelectAll().Separator(LIGHT);
    table.SelectRow(0).Decorate(bold);
    table.SelectRow(0).SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(LIGHT);
    table.SelectAll().DecorateCells(center);
    
    return table.Render() | color(Colors::secondary());
}

Element StyledStockTable(const std::vector<std::string>& headers) {
    if (datasetBarang.empty()) {
        return text("Tidak ada data") | center | dim;
    }
    
    // Extended headers to include HargaJual and Expired
    std::vector<std::string> extendedHeaders = {"Nama", "Kode", "Stock", "H.Beli", "H.Jual", "Expired"};
    
    std::vector<std::vector<Element>> tableData;
    
    std::vector<Element> headerRow;
    for (const auto& h : extendedHeaders) {
        headerRow.push_back(text(" " + h + " ") | bold | color(Colors::accent()));
    }
    tableData.push_back(headerRow);
    
    for (const auto& b : datasetBarang) {
        std::vector<Element> tableRow;
        
        // Get expired status for color coding
        TUIExpiredStatus expStatus = getTUIExpiredStatus(b.expDay, b.expMonth, b.expYear);
        bool outOfStock = (b.jumlahBarang == 0);
        
        // Nama barang - merah jika habis atau expired
        if (outOfStock) {
            tableRow.push_back(text(" " + b.nama + " ") | color(Colors::danger()) | bold);
        } else if (expStatus == TUIExpiredStatus::EXPIRED) {
            tableRow.push_back(text(" " + b.nama + " ") | color(Colors::danger()));
        } else if (expStatus == TUIExpiredStatus::NEAR_EXPIRED) {
            tableRow.push_back(text(" " + b.nama + " ") | color(Color::RGB(255, 193, 7))); // Yellow/warning
        } else {
            tableRow.push_back(text(" " + b.nama + " "));
        }
        
        // Kode barang
        tableRow.push_back(text(" " + std::to_string(b.codeBarang) + " "));
        
        // Stock - merah jika 0
        if (outOfStock) {
            tableRow.push_back(text(" 0 ") | color(Colors::danger()) | bold);
        } else {
            tableRow.push_back(text(" " + std::to_string(b.jumlahBarang) + " "));
        }
        
        // Harga Beli
        tableRow.push_back(text(" " + formatRupiah(b.hargaBarang) + " "));
        
        // Harga Jual - show effective price with flash sale indicator
        FlashSaleInfo saleInfo = getFlashSaleInfo(b);
        if (saleInfo.isOnSale) {
            // Show sale price in magenta with [SALE] indicator
            std::string saleText = formatRupiah(saleInfo.effectivePrice) + " [SALE]";
            tableRow.push_back(text(" " + saleText + " ") | color(Color::RGB(255, 0, 255)) | bold); // Magenta
        } else {
            // Show regular price in green
            tableRow.push_back(text(" " + formatRupiah(b.hargaJual) + " ") | color(Colors::success()));
        }
        
        // Expired date with status indicator
        std::string expiredStr = formatExpiredDateTUI(b.expDay, b.expMonth, b.expYear);
        if (expStatus == TUIExpiredStatus::EXPIRED) {
            tableRow.push_back(text(" " + expiredStr + " ") | color(Colors::danger()));
        } else if (expStatus == TUIExpiredStatus::NEAR_EXPIRED) {
            tableRow.push_back(text(" " + expiredStr + " ") | color(Color::RGB(255, 193, 7)));
        } else {
            tableRow.push_back(text(" " + expiredStr + " "));
        }
        
        tableData.push_back(tableRow);
    }
    
    auto table = Table(tableData);
    
    table.SelectAll().Border(LIGHT);
    table.SelectAll().Separator(LIGHT);
    table.SelectRow(0).Decorate(bold);
    table.SelectRow(0).SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(LIGHT);
    table.SelectAll().DecorateCells(center);
    
    return table.Render() | color(Colors::secondary());
}

Element StatusBar(const std::string& message) {
    return hbox({
        text(" "),
        text(message) | color(Colors::light()),
        text(" ") | flex,
    }) | bgcolor(Colors::dark());
}

} // namespace TUI

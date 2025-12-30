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
    
    std::vector<std::vector<Element>> tableData;
    
    std::vector<Element> headerRow;
    for (const auto& h : headers) {
        headerRow.push_back(text(" " + h + " ") | bold | color(Colors::accent()));
    }
    tableData.push_back(headerRow);
    
    for (const auto& b : datasetBarang) {
        std::vector<Element> tableRow;
        bool outOfStock = (b.jumlahBarang == 0);
        
        // Nama barang - merah jika habis
        if (outOfStock) {
            tableRow.push_back(text(" " + b.nama + " ") | color(Colors::danger()) | bold);
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
        
        // Harga
        tableRow.push_back(text(" " + formatRupiah(b.hargaBarang) + " "));
        
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

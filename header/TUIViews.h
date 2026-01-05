#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_VIEWS_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_VIEWS_H

#include "TUIComponents.h"
#include "TUIHelpers.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

namespace TUI {

// Render functions for each view state
ftxui::Element RenderMainMenu(ftxui::Component& menu);
ftxui::Element RenderKasirMenu(ftxui::Component& menu, const std::vector<std::string>& stockHeaders, const std::vector<std::string>& cartHeaders);
ftxui::Element RenderKasirBelanja(ftxui::Component& codeInput, ftxui::Component& jumlahInput, const std::vector<std::string>& stockHeaders, const std::vector<std::string>& cartHeaders);
ftxui::Element RenderCheckout(const std::vector<std::string>& cartHeaders);
ftxui::Element RenderStockMenu(ftxui::Component& menu, const std::vector<std::string>& stockHeaders);
ftxui::Element RenderStockAdd(ftxui::Component& namaInput, ftxui::Component& jumlahInput, ftxui::Component& hargaInput, const std::vector<std::string>& stockHeaders);
ftxui::Element RenderStockEdit(ftxui::Component& codeInput, ftxui::Component& namaInput, ftxui::Component& jumlahInput, ftxui::Component& hargaInput, const std::vector<std::string>& stockHeaders);
ftxui::Element RenderStockDelete(ftxui::Component& codeInput, const std::vector<std::string>& stockHeaders);
ftxui::Element RenderDeleteConfirm(const std::string& itemName, int itemCode);

// Keuangan views
ftxui::Element RenderKeuanganMenu(ftxui::Component& menu);
ftxui::Element RenderKeuanganRingkasan();
ftxui::Element RenderKeuanganPemasukan(const std::vector<std::string>& headers);
ftxui::Element RenderKeuanganPengeluaran(const std::vector<std::string>& headers);
ftxui::Element RenderKeuanganExportResult(const std::string& filePath);

}

#endif

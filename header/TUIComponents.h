#ifndef IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_COMPONENTS_H
#define IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_COMPONENTS_H

#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

namespace TUI {

// Color palette untuk styling
namespace Colors {
    using namespace ftxui;
    
    inline Color primary() { return Color::RGB(66, 135, 245); }
    inline Color secondary() { return Color::RGB(0, 188, 212); }
    inline Color accent() { return Color::RGB(255, 193, 7); }
    inline Color success() { return Color::RGB(76, 175, 80); }
    inline Color danger() { return Color::RGB(244, 67, 54); }
    inline Color dark() { return Color::RGB(33, 33, 33); }
    inline Color light() { return Color::RGB(250, 250, 250); }
}

// UI Component functions
ftxui::Element Logo();
ftxui::Element StyledHeader(const std::string& title);
ftxui::Element StyledBox(ftxui::Element content, const std::string& title = "");
ftxui::Element StyledTable(const std::vector<std::vector<std::string>>& data, 
                           const std::vector<std::string>& headers);
ftxui::Element StyledStockTable(const std::vector<std::string>& headers);
ftxui::Element StatusBar(const std::string& message);

}

#endif //IVENTARIS_DAN_KEUANGAN_KONSEP_TUI_COMPONENTS_H

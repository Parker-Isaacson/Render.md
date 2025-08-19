#include "colors.h"

// Convert hex substring to int
int hexToInt(const std::string& hex) {
    return std::stoi(hex, nullptr, 16);
}

// Clamp value between 0 and 255
int clamp(int value) {
    return std::max(0, std::min(255, value));
}

// Convert int to two-character hex string
std::string intToHex(int value) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << clamp(value);
    return ss.str();
}

// Darken a hex color by a given percentage (0.0 to 1.0)
std::string darkenColor(const std::string& hexColor, double percent) {
    if (hexColor.size() != 7 || hexColor[0] != '#') {
        throw std::invalid_argument("Invalid hex color format");
    }

    int r = hexToInt(hexColor.substr(1, 2));
    int g = hexToInt(hexColor.substr(3, 2));
    int b = hexToInt(hexColor.substr(5, 2));

    // Reduce brightness by the given percent
    r = static_cast<int>(r * (1.0 - percent));
    g = static_cast<int>(g * (1.0 - percent));
    b = static_cast<int>(b * (1.0 - percent));

    return "#" + intToHex(r) + intToHex(g) + intToHex(b);
}
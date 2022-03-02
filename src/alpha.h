#pragma once

#include <array>
#include <locale>

namespace impl {

inline auto locale = std::locale{"sv_SE.UTF8"};

}

inline bool isUtfTail(char c) {
    // 0xC0 = 0b11000000
    // 0x80 = 0b10000000
    return ((c & 0xC0) == 0x80);
}

inline bool isSwedishSpecialChar(char c) {
    constexpr auto swedishChars = std::array{
        "å"[0],
        "ä"[0],
        "ö"[0],
        "é"[0],
        "Å"[0],
        "Ä"[0],
        "É"[0],
    };
    for (auto sc : swedishChars) {
        if (sc == c) {
            return true;
        }
    }
    return false;
}

inline bool isAlphaNoSpace(char c) {
    return (std::isalpha(c, impl::locale) || isUtfTail(c) ||
            isSwedishSpecialChar(c)) &&
           !std::isspace(c);
}

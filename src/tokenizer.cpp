#include "tokenizer.h"
#include "alpha.h"
#include <fstream>

std::vector<std::string> tokenize(std::filesystem::path path) {
    auto words = std::vector<std::string>{};
    auto file = std::ifstream{path};

    bool isWord = false;

    for (char c = 0; file.read(&c, 1);) {
        if (auto a = isAlphaNoSpace(c); a != isWord || words.empty()) {
            isWord = a;
            words.emplace_back();
        }

        words.back().push_back(c);
    }

    return words;
}

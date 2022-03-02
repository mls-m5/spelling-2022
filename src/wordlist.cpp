#include "wordlist.h"
#include <fstream>
#include <iostream>

WordList::WordList(std::filesystem::path path) {
    auto file = std::ifstream{path};
    if (!file.is_open()) {
        auto err = "could not load wordlist";
        std::cerr << err << "\n";
        throw std::runtime_error{err};
    }

    for (std::string line; getline(file, line);) {
        _words.insert(line);
    }
}

#include "corrections.h"
#include "alpha.h"
#include <fstream>

Corrections::Corrections(std::filesystem::path path) {
    auto file = std::ifstream{path};

    for (std::string line; getline(file, line);) {
        auto s = split(line, "->");

        if (s) {
            auto a = trim(s->first);
            auto b = trim(s->second);

            _corrections.insert({a, b});
        }
    }
}

std::string Corrections::correct(const std::string str) {
    if (auto f = _corrections.find(str); f != _corrections.end()) {
        return f->second;
    }

    return {};
}

#include "tokenizer.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

class Token {
public:
    enum Type {
        None,
        Word,
        Space,
    };

    std::string_view str() {
        return _str;
    }

private:
    Type _type = None;
    std::string _str;
};

int main(int argc, char **argv) {
    auto args = std::vector<std::filesystem::path>(argv + 1, argv + argc);

    auto dir = args.at(0);

    for (const auto &file :
         std::filesystem::recursive_directory_iterator{dir}) {
        std::cout << file << "\n";

        auto tokens = tokenize(file);

        for (const auto &token : tokens) {
            std::cout << "\t'" << token << "'\n";
        }
    }
}

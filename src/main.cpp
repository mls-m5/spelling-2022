#include "database.h"
#include "tokenizer.h"
#include "wordlist.h"
#include <iostream>

int main(int argc, char **argv) {
    auto args = std::vector<std::filesystem::path>(argv + 1, argv + argc);
    auto dir = args.at(0);
    auto database = Database{};

    for (const auto &file :
         std::filesystem::recursive_directory_iterator{dir}) {
        std::cout << file << "\n";

        auto strings = tokenize(file);
        auto tokens = database.createTokens(std::move(strings));

        for (const auto &token : tokens) {
            std::cout << "\t'" << *token << "'\t - " << token.use_count();

            std::cout << "\n";
        }

        std::cout.flush();

        for (const auto &token : tokens) {
            std::cout << *token;
        }

        std::cout << "\n";
    }
}

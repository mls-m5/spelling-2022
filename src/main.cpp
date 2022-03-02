#include "database.h"
#include "tokenizer.h"
#include "wordlist.h"
#include <iostream>

int main(int argc, char **argv) {
    auto args = std::vector<std::filesystem::path>(argv + 1, argv + argc);
    auto dir = args.at(0);
    auto database = Database{};
    auto wordList = WordList{"dict.txt"};

    auto verbose = false;

    for (const auto &file :
         std::filesystem::recursive_directory_iterator{dir}) {
        std::cout << file << "\n";

        if (file.path().extension() != ".md") {
            std::cout << "not md file, skipping\n";
            continue;
        }

        auto strings = tokenize(file);
        auto tokens = database.createTokens(std::move(strings));

        if (verbose) {
            for (const auto &token : tokens) {
                std::cout << "\t'" << *token << "'\t - " << token.use_count();

                if (isAlphaNoSpace(token->front())) {
                    if (!wordList.exists(*token)) {
                        std::cout << " - not in dictionary";
                    }
                }

                std::cout << "\n";
            }

            std::cout.flush();

            for (const auto &token : tokens) {
                std::cout << *token;
            }

            std::cout << "\n";
        }
    }

    std::cout << "misspelled words:\n";

    for (auto &word : database.getTokens(
             [&](auto &&word) { return !wordList.exists(word); })) {
        std::cout << *word << "\t" << word.use_count() << "\n";
    }
}

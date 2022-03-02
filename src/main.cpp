#include "corrections.h"
#include "database.h"
#include "tokenizer.h"
#include "wordlist.h"
#include <iostream>

struct File {
    std::vector<std::shared_ptr<Token>> tokens;
};

int main(int argc, char **argv) {
    auto args = std::vector<std::filesystem::path>(argv + 1, argv + argc);
    auto dir = args.at(0);
    auto database = Database{};
    auto wordList = WordList{"spell/sv.dict.txt"};
    wordList.loadWords("spell/names.txt");
    wordList.loadWords("spell/sv.utf-8.add");

    auto corrections = Corrections{"spell/sv.corrections.txt"};
    auto nameCorrections = Corrections{"spell/names.txt", true};

    auto verbose = false;

    auto files = std::vector<File>{};

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

        files.push_back({std::move(tokens)});
    }

    std::cout << "misspelled words:\n";

    auto missSpelled =
        database.getTokens([&](auto &&word) { return !wordList.exists(word); });

    for (auto &word : missSpelled) {
        std::cout << *word;
        if (auto correction = nameCorrections.correct(*word);
            !correction.empty()) {
            std::cout << " (name) -> '" << correction << "'";
        }
        if (auto correction = corrections.correct(*word); !correction.empty()) {
            std::cout << " -> '" << correction << "'";
        }
        std::cout << "\t" << word.use_count() << "\n";
    }

    std::cout << "common missspellings\n";
    for (auto &word : missSpelled) {
        if (word.use_count() > 3) {
            std::cout << *word << "\t" << word.use_count() << "\n";
        }
    }
}

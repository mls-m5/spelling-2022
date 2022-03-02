#include "corrections.h"
#include "database.h"
#include "tokenizer.h"
#include "wordlist.h"
#include <iostream>

struct File {
    std::vector<std::shared_ptr<Token>> tokens;
    std::filesystem::path path;

    std::string context(Token *t) {
        size_t previousIndex = 0;

        for (size_t i = 0; i < tokens.size(); ++i) {
            auto &token = tokens.at(i);
            if (token->find('\n') != std::string::npos) {
                previousIndex = i + 1;
            }

            if (token.get() == t) {
                auto str = std::string{};

                for (size_t j = previousIndex; j <= i + 3 && j < tokens.size();
                     ++j) {
                    if (i == j) {
                        str += "-->" + *tokens.at(j) + "<--";
                    }
                    else {
                        str += *tokens.at(j);
                    }
                }
                return str;
            }
        }

        return "";
    }
};

int main(int argc, char **argv) {
    auto args = std::vector<std::filesystem::path>(argv + 1, argv + argc);
    auto dir = args.at(0);
    auto database = Database{};
    auto wordList = WordList{"spell/sv.dict.txt"};
    wordList.loadWords("spell/names.txt");
    wordList.loadWords("spell/sv.utf-8.add");
    wordList.loadWords("spell/en.dict.txt");

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

        files.push_back({std::move(tokens), file.path().filename()});
    }

    std::cout << "misspelled words:\n";

    auto missSpelled =
        database.getTokens([&](auto &&word) { return !wordList.exists(word); });

    for (auto &word : missSpelled) {
        std::cout << "\t -->" << *word;
        if (auto correction = nameCorrections.correct(*word);
            !correction.empty()) {
            std::cout << " (name) -> '" << correction << "'";
        }
        if (auto correction = corrections.correct(*word); !correction.empty()) {
            std::cout << " -> '" << correction << "'";
        }

        std::cout << "\t" << word.use_count() << "\n";

        std::cout << "\t";
        for (auto &file : files) {
            if (auto context = file.context(word.get()); !context.empty()) {
                std::cout << file.path;
                std::cout << ": " << context << " ";
            }
        }
        std::cout << "\n";
    }

    std::cout << missSpelled.size() << " misspelled words\n";

    if (verbose) {
        std::cout << "common missspellings\n";
        for (auto &word : missSpelled) {
            if (word.use_count() > 3) {
                std::cout << *word << "\t" << word.use_count() << "\n";
            }
        }
    }
}

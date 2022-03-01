#include "tokenizer.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class Token {
public:
    enum Type {
        None,
        Word,
        Space,
    };

    Token(Type type, std::string str)
        : _type{type}
        , _str{std::move(str)} {}

    std::string_view str() {
        return _str;
    }

private:
    Type _type = None;
    std::string _str;
};

struct Database {
    std::unordered_map<std::string, std::shared_ptr<Token>> _words;

    std::shared_ptr<Token> createPointer(std::string_view word) {
        if (word.empty()) {
            return {};
        }

        if (auto c = word.front(); isalpha(c) && !isspace(c)) {
            const auto str = std::string{word};
            if (auto f = _words.find(str); f != _words.end()) {
                return f->second;
            }

            auto token = std::make_shared<Token>(Token::Word, str);

            _words.insert(std::pair{str, token});

            return token;
        }

        // Just create something and dont save it
        return std::make_shared<Token>(Token::Space, std::string{word});
    }

    auto createTokens(std::vector<std::string> strings) {
        auto tokens = std::vector<std::shared_ptr<Token>>{};

        for (const auto &str : strings) {
            tokens.push_back(createPointer(str));
        }

        return tokens;
    }
};

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
            std::cout << "\t'" << token->str() << "\t - " << token.use_count()
                      << "'\n";
        }
    }
}

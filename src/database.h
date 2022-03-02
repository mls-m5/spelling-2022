#pragma once

#include "alpha.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// class Token {
// public:
//     enum Type {
//         None,
//         Word,
//         Space,
//     };

//    Token(Type type, std::string str)
//        : _type{type}
//        , _str{std::move(str)} {}

//    std::string_view str() {
//        return _str;
//    }

// private:
//     Type _type = None;
//     std::string _str;
// };

using Token = std::string;

struct Database {
    std::unordered_map<std::string, std::shared_ptr<Token>> _words;

    std::shared_ptr<Token> createPointer(const std::string &word) {
        if (word.empty()) {
            return {};
        }

        if (auto c = word.front(); isAlphaNoSpace(c)) {
            if (auto f = _words.find(word); f != _words.end()) {
                return f->second;
            }

            //            auto token = std::make_shared<Token>(Token::Word,
            //            str);
            auto token = std::make_shared<Token>(word);
            _words.insert(std::pair{word, token});
            return token;
        }

        // Just create something and dont save it
        //        return std::make_shared<Token>(Token::Space,
        //        std::string{word});
        return std::make_shared<Token>(word);
    }

    auto createTokens(std::vector<std::string> strings) {
        auto tokens = std::vector<std::shared_ptr<Token>>{};

        for (const auto &str : strings) {
            tokens.push_back(createPointer(str));
        }

        return tokens;
    }
};

#pragma once

#include "alpha.h"
#include <algorithm>
#include <filesystem>
#include <string>
#include <unicode/unistr.h>
#include <unordered_set>

// Verified words
struct WordList {
    WordList(std::filesystem::path path);

    void loadWords(std::filesystem::path path);

    bool exists(const std::string &word, bool caseSensitive = false) {
        if (caseSensitive) {
            return _words.find(word) != _words.end();
        }

        if (_words.find(word) != _words.end()) {
            return true;
        }

        return _words.find(toLower(word)) != _words.end();
    }

    void add(std::string word) {
        _words.insert(word);
    }

private:
    std::unordered_set<std::string> _words;
};

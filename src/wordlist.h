#pragma once

#include <filesystem>
#include <string>
#include <unordered_set>

// Verified words
struct WordList {
    WordList(std::filesystem::path path);

    bool exists(const std::string &word) {
        return _words.find(word) != _words.end();
    }

    void add(std::string word) {
        _words.insert(word);
    }

private:
    std::unordered_set<std::string> _words;
};

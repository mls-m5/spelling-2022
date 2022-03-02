#pragma once

#include <filesystem>
#include <string>
#include <unordered_set>

// Verified words
struct WordList {
    WordList(std::filesystem::path path) {}

    bool exists(const std::string &word) {
        return _map.find(word) != _map.end();
    }

    void add(std::string word) {
        _map.insert(word);
    }

private:
    std::unordered_set<std::string> _map;
};

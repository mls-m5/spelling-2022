#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

class Corrections {
public:
    Corrections(std::filesystem::path path);

    std::string correct(const std::string str);

private:
    std::unordered_map<std::string, std::string> _corrections;
};

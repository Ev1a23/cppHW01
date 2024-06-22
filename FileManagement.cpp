#include "FileManagement.h"
#include <fstream>
#include <stdexcept>

void FileManagement::saveOutput(const std::string& path, const std::vector<std::string>& data) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + path);
    }

    for (const auto& line : data) {
        file << line << std::endl;
    }

    file.close();
}

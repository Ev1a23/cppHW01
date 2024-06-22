#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <string>
#include <vector>
#include "House.h"

class FileManagement {
public:
    static House loadHouse(const std::string& path);
    static void saveOutput(const std::string& path, const std::vector<std::string>& data);
};

#endif // FILEMANAGEMENT_H
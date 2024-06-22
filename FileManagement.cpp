#include "FileManagement.h"
class FileManagement {
public:
    static House loadHouse(std::string path);
    static void saveOutput(std::string path, const std::vector<std::string>& data);
};
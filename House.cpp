#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

class House {
public:
    House(const std::string& inputFile) {
        loadHouse(inputFile);
    }

    int getDirtLevel(int x, int y) const {
        return grid[x][y];
    }

    bool isWall(int x, int y) const {
        return grid[x][y] == -1;
    }

    std::pair<int, int> getDockingStation() const {
        return dockingStation;
    }

private:
    std::vector<std::vector<int>> grid;
    std::pair<int, int> dockingStation;

    void loadHouse(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open input file.");
        }

        std::string line;
        int x = 0;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<int> row;
            int value;
            char comma;

            int y = 0;
            while (ss >> value) {
                if (value == 10) {
                    dockingStation = {x, y};
                }
                row.push_back(value);
                if (ss >> comma && comma != ',') {
                    throw std::runtime_error("Invalid file format");
                }
                y++;
            }
            grid.push_back(row);
            x++;
        }
    }
};

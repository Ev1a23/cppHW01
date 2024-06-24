#include "House.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

House::House(const std::string& inputFile) {
    loadHouse(inputFile);
}

int House::getDirtLevel(int x, int y) const {
    return grid[x][y];
}

bool House::isWall(int x, int y) const {
    return grid[x][y] == -1;
}

std::pair<int, int> House::getDockingStation() const {
    return dockingStation;
}

int House::getTotalDirt() const {
    return totalDirt;
}

// double House::getMaxBatterySteps() const {
//     return maxBatterySteps;
// }

int House::getMaxAllowedSteps() const {
    return maxAllowedSteps;
}

VacuumCleaner House::getCleaner() const {
    return cleaner;
}

void House::cleanPos(int x, int y) {
    grid[x][y] = grid[x][y] - 1;
    cleaner.clean();
}

void House::loadHouse(const std::string& path) {
    double maxBatterySteps;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open input file.");
    }

    std::string line;
    if (std::getline(file, line)) {
        auto pos = line.find(":");
        if (pos != std::string::npos) {
            maxBatterySteps = std::stod(line.substr(pos + 1));
        }
    }
    if (std::getline(file, line)) {
        auto pos = line.find(":");
        if (pos != std::string::npos) {
            maxAllowedSteps = std::stoi(line.substr(pos + 1));
        }
    }
    int x = 0;
    int maxWidth = 0;
    std::vector<std::vector<int>> tempGrid;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> row;
        int value;
        char comma;

        int y = 0;
        while (ss >> value) {
            if (value == 10) {
                dockingStation = {x + 1, y + 1};  // Temporarily store initial docking position corresponding to adding walls
                value = 0;  // Treat the docking station like clean floor
            }
            if(value > 0 && value < 10) {
                totalDirt += value;
            }
            row.push_back(value);
            if (ss >> comma && comma != ',') {
                throw std::runtime_error("Invalid file format");
            }
            y++;
        }
        maxWidth = std::max(maxWidth, y);  // Find the maximum width
        tempGrid.push_back(row);
        x++;
    }

    // Normalize widths and add walls at left and right if necessary
    for (auto& row : tempGrid) {
        if (row.size() < static_cast<std::vector<int>::size_type>(maxWidth)) {
            row.resize(maxWidth, -1);  // Pad with walls if row is shorter
        }
        // Add left and right walls
        row.insert(row.begin(), -1);
        row.push_back(-1);
    }

    // Adjust the size for top and bottom walls after adding side walls
    std::vector<int> wallRow(maxWidth + 2, -1); // +2 for the walls added on both sides
    grid.push_back(wallRow); // Top wall
    for (auto& row : tempGrid) {
        grid.push_back(row);
    }

    cleaner = VacuumCleaner(maxBatterySteps, dockingStation);
};
   

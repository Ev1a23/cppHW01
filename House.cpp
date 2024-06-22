#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

class House {
public:
    House(const std::string& inputFile) {
        loadHouse(inputFile);
        adjustDockingStationPosition();
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

    int getTotalDirt() const {
        return totalDirt;
    }

    int getMaxBatterySteps() const {
        return maxBatterySteps;
    }

    int getMaxAllowedSteps() const {
        return maxAllowedSteps;
    }

private:
    std::vector<std::vector<int>> grid;
    std::pair<int, int> dockingStation;
    int totalDirt = 0;
    int maxBatterySteps = 0;
    int maxAllowedSteps = 0;

    void loadHouse(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open input file.");
        }

        std::string line;
        if (std::getline(file, line)) {
            auto pos = line.find(":");
            if (pos != std::string::npos) {
                maxBatterySteps = std::stoi(line.substr(pos + 1));
            }
        }
        if (std::getline(file, line)) {
            auto pos = line.find(":");
            if (pos != std::string::npos) {
                maxAllowedSteps = std::stoi(line.substr(pos + 1));
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
            if (row.size() < maxWidth) {
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
        grid.push_back(wallRow); // Bottom wall
    }

    void adjustDockingStationPosition() {
        // Adjust the docking station position to account for the added wall on the left and potentially top
        dockingStation.first += 1;  // One position right due to the left wall
        dockingStation.second += 1;  // One position down due to the top wall
    }
};

#include "House.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

House::House(){}

House::House(const std::string& inputFile) : cleaner(-1, -1, {-1,-1}){
    loadHouse(inputFile);
}

void House::clean(){
    std::pair<int,int> pos = cleaner.getPosition();
    grid[pos.first][pos.second]--;
    cleaner.clean();
}


//enum class Step { North, East, South, West, Stay, Finish };
std::pair<int,int> House::moveTranslation(Step step) const
{
    std::pair<int,int> diff;
	switch(step)
	{
		case Step::North:
			diff = {-1,0};
            break;
		case Step::East:
			diff = {0,1};
            break;
		case Step::South:
			diff = {1,0};
            break;
		case Step::West:
			diff = {0,-1};
            break;
		default:
			diff = {0,0};
            break;
	}
    return {cleaner.getPosition().first + diff.first, cleaner.getPosition().second + diff.second};
}


House::VacuumCleaner& House::getCleaner(){
    return cleaner;
}

// House::SensorSystem& House::getSensors(){
//     return sensors;
// }

bool House::isWall(int x, int y) const {
    return grid[x][y] == -1;
}

std::pair<int, int> House::getDockingStation() const {
    return dockingStation;
}

// int House::getTotalDirt() const {
//     return totalDirt;
// }

House::VacuumCleaner::VacuumCleaner(){}


House::VacuumCleaner::VacuumCleaner(const std::size_t maxAllowedSteps, const double maxBattery, const std::pair<int, int> startPosition)
        : maxAllowedSteps(maxAllowedSteps), battery(maxBattery), maxBattery(maxBattery), position(startPosition) {}


std::size_t House::VacuumCleaner::getMaxBatterySteps() const {
    return maxBattery;
}

std::size_t House::VacuumCleaner::getMaxAllowedSteps() const {
    return maxAllowedSteps;
}

std::pair<int,int> House::VacuumCleaner::getPosition() const {
    return position;
}

void House::VacuumCleaner::move(int x, int y) {
    position.first = x;
    position.second = y;
    battery--;
}

void House::VacuumCleaner::clean(){
    battery--;
}

void House::VacuumCleaner::charge(){
    double rechargeAmount = maxBattery / 20.0;
    battery += rechargeAmount;
    if (battery > maxBattery)
    {
        battery = maxBattery;
    }
}

double House::VacuumCleaner::batteryLevel() const {return battery;};


// House::SensorSystem::SensorSystem(House& house) : house(house){}

// int House::SensorSystem::dirtSensor() const {
//     std::pair<int,int> pos = house.getCleaner().getPosition();
//     return house.grid[pos.first][pos.second];
// }

// double House::SensorSystem::batterySensor() const {
//     return house.getCleaner().batteryLevel();
// }

// std::vector<std::pair<int, int>> House::SensorSystem::wallsSensor() const{
//     std::pair<int,int> cleanerPos = house.getCleaner().getPosition();
//     std::vector<std::pair<int, int>> nonWallLocations;
 
//     // Check each direction for non-wall positions
//     if (!house.isWall(cleanerPos.first, cleanerPos.second - 1)) {
//         nonWallLocations.push_back({cleanerPos.first, cleanerPos.second - 1});  // North
//     }
//     if (!house.isWall(cleanerPos.first + 1, cleanerPos.second)) {
//         nonWallLocations.push_back({cleanerPos.first + 1, cleanerPos.second});  // East
//     }
//     if (!house.isWall(cleanerPos.first, cleanerPos.second + 1)) {
//         nonWallLocations.push_back({cleanerPos.first, cleanerPos.second + 1});  // South
//     }
//     if (!house.isWall(cleanerPos.first - 1, cleanerPos.second)) {
//         nonWallLocations.push_back({cleanerPos.first - 1, cleanerPos.second});  // West
//     }
 
//     return nonWallLocations;
// }



static void removeSpaces(std::string& str) {
    // Use the erase-remove idiom to remove all spaces from the string
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}


static int getValue(std::ifstream& file, const std::string& var_name)
{
    int val = -1;
    std::string line;
    if (std::getline(file, line)) {
        auto pos = line.find("=");
        if (pos != std::string::npos && line.substr(0, var_name.length()) == var_name) {
            std::string val_string = line.substr(pos + 1);
            removeSpaces(val_string);
            val = std::stoi(val_string);
			if(val < 0)
			{
				throw std::runtime_error("Invalid " + var_name);
			}
        }
		else
		{
			throw std::runtime_error("Invalid file format, " + var_name + " not found");
		}
    }
    return val;
}


void House::loadHouse(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open input file.");
    }

    std::string line;

    // Title
    std::getline(file, line);
    
    // load house itself

    size_t maxSteps = (size_t)getValue(file, "MaxSteps");
    int maxBattery = getValue(file, "MaxBattery");
    int rows = getValue(file, "Rows");
    int cols = getValue(file, "Cols");


    std::cout << "Max steps: " << maxSteps << " MaxBattery: " << maxBattery << " Rows: " << rows << " Cols" << cols << "\n";
    
    int x = 0;
    // int maxWidth = 0;
    std::vector<std::vector<int>> tempGrid;

    // totalDirt = 0;
    while (std::getline(file, line) && x < rows) {
        std::stringstream ss(line);
        std::vector<int> row;
        char value;
        int  val_int;
        int y = 0;
        while (size_t(y) < line.length() && y < cols) {
            value = line[y];
            val_int = 0;
            if (value == 'D') {
				if(dockingStation.first != -1 && dockingStation.second != -1) {
					throw std::runtime_error("Duplicate docking station found");
				}
                dockingStation = {x + 1, y + 1};  // Temporarily store initial docking position corresponding to adding walls
                // value = 0;  // Treat the docking station like clean floor
            }
            if(value >= '0' && value <= '9') {
                val_int = value - 48; // ascii val of '0'
            }
            if(value == 'W') {
                val_int = -1;
            }
            row.push_back(val_int);
            y++;
        }
        tempGrid.push_back(row);
        x++;
    }

    std::vector<int> row(cols, 0);
    for(int i = x; i < rows; i++)
    { // fill extra rows if needed
        tempGrid.push_back(row);
    }

    // Normalize widths and add walls at left and right if necessary
    for (auto& row : tempGrid) {
        if (row.size() < size_t(cols)) {
            row.resize(cols, 0);  // Pad with walls if row is shorter
        }
        // Add left and right walls
        row.insert(row.begin(), -1);
        row.push_back(-1);
    }

    // Adjust the size for top and bottom walls after adding side walls
    std::vector<int> wallRow(cols + 2, -1); // +2 for the walls added on both sides
    grid.push_back(wallRow); // Top wall
    for (auto& row : tempGrid) {
        grid.push_back(row);
    }
	grid.push_back(wallRow); // Bottom wall

    for (const auto& row : grid) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

	if(dockingStation.first == -1 && dockingStation.second == -1) {
		throw std::runtime_error("No docking station found");
	}

    cleaner = VacuumCleaner(maxSteps, maxBattery, dockingStation);
	file.close();
};


int House::totalDirt()
{
    int sum = 0;
    for (const auto& row : grid) {
        for (const auto& value : row) {
            if (value != -1) {
                sum += value;
            }
        }
    }
    return sum;
}

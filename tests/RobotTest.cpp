#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <stdexcept>

class RobotTest : public ::testing::Test {
protected:
    std::string command;
    std::string outputFilePath = "output.txt"; // Path where your program writes its output
    std::string errorFilePath = "error.txt";   // Path for capturing stderr

    // Function to run the binary with a given input file
    void runMyRobot(const std::string& inputPath, int expectedReturnVal, const std::string& expectedOutput, const std::string& expectedException) {
        // Build the command to redirect stdout and stderr to files
        command = "../myrobot " + inputPath + " > " + outputFilePath + " 2>" + errorFilePath;
        int result = std::system(command.c_str()) >> 8;

        // Check return value
        ASSERT_EQ(result, expectedReturnVal) << "Expected return value mismatch for command: " << command;

        // Check the last line of output
        if (!expectedOutput.empty()) {
            std::string lastLine = getLastLine(outputFilePath);
            ASSERT_EQ(lastLine, expectedOutput) << "Output mismatch detected.";
        }

        // Check for expected exception message, if any
        if (!expectedException.empty()) {
            std::ifstream errFile(errorFilePath);
            std::stringstream errBuffer;
            errBuffer << errFile.rdbuf();
            ASSERT_TRUE(errBuffer.str().find(expectedException) != std::string::npos)
                << "Expected exception message not found. Actual message: " << errBuffer.str();
        }
    }

    // Helper function to get the last line from a file
    std::string getLastLine(const std::string& filePath) {
        std::ifstream file(filePath);
        std::string line;
        std::string lastLine;
        while (getline(file, line)) {
            lastLine = line;
        }
        return lastLine;
    }
};

TEST_F(RobotTest, TestInput1) {
    std::string inputPath = "Test_1_Basic_Functionality_Test.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Success";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput2) {
    std::string inputPath = "Test_2_House_Not_Surrounded_by_Walls.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Success";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput3) {
    std::string inputPath = "Test_3_Insufficient_Battery.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput4) {
    std::string inputPath = "Test_4_Maximum_Steps_Exceeded.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput5) {
    std::string inputPath = "Test_5_docking_station_surrounded_by_dirt.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput6) {
    std::string inputPath = "Test_6_irregular_shape.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput7) {
    std::string inputPath = "Test_7_completely_clean_house.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Success";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput8) {
    std::string inputPath = "Test_8_maximum_dirt_levels.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Success";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput9) {
    std::string inputPath = "Test_9_narrow_corridors.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput10) {
    std::string inputPath = "Test_10_large_open_area.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput11) {
    std::string inputPath = "Test_11_obstacle_course.txt";
    int expectedReturnVal = 1;
    std::string expectedOutput = "";
    std::string expectedException = "Invalid value found in house";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput12) {
    std::string inputPath = "Test_12_sparse_dirt.txt";
    int expectedReturnVal = 0;
    std::string expectedOutput = "Mission Status: Failure";
    std::string expectedException = "";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput13) {
    std::string inputPath = "Test_13_multiple_docking_stations.txt";
    int expectedReturnVal = 1;
    std::string expectedOutput = "";
    std::string expectedException = "Duplicate docking station found";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput14) {
    std::string inputPath = "Test_14_Invalid_Format.txt";
    int expectedReturnVal = 1;
    std::string expectedOutput = "";
    std::string expectedException = "Invalid file format";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput15) {
    std::string inputPath = "Test_15_No_Docking.txt";
    int expectedReturnVal = 1;
    std::string expectedOutput = "";
    std::string expectedException = "No docking station found";
    runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput16) {
	std::string inputPath = "Test_16_No_Max_Battery.txt";
	int expectedReturnVal = 1;
	std::string expectedOutput = "";
	std::string expectedException = "Invalid file format, max battery steps not found";
	runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

TEST_F(RobotTest, TestInput17) {
	std::string inputPath = "Test_17_Negative_Max_Battery.txt";
	int expectedReturnVal = 1;
	std::string expectedOutput = "";
	std::string expectedException = "Invalid max battery steps";
	runMyRobot(inputPath, expectedReturnVal, expectedOutput, expectedException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
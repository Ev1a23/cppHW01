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
        int result = std::system(command.c_str());

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
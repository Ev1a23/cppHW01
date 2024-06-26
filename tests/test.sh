#!/bin/bash

# Directory containing the test files
test_dir="./"

# Find all test files matching the pattern Test_1_<testcase>.txt
test_files=$(ls ${test_dir}Test_*.txt)

my_robot_path=../myrobot
# Loop through each test file and run the program

output_folder=./output/


for test_file in $test_files; do
  echo "Running test: $test_file"
  output_file="$output_folder${test_file%.txt}.output"
  # Run the vacuum cleaner simulation program and store the output
  $my_robot_path "$test_file" > "$output_file"
  echo "Test $test_file completed. Output saved to $output_file."
  echo "----------------------------"
done

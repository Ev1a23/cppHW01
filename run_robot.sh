#!/bin/bash

# Number of iterations
iterations=10000
# Command to run
command="./myrobot input.txt"
cn=0

# Run the command in a loop
for ((i = 1; i <= iterations; i++)); do
    if [ $((i % 100)) -eq 0 ]; then
        echo "Iteration $i"
    fi
    # Run the command and suppress its output
    $command > /dev/null 2>&1
    # Capture the exit status immediately after the command
    exit_status=$?
    if [ $exit_status -ne 0 ]; then
        echo "Iteration $i: Error occured"
        cn=$((cn + 1))
    fi
done

echo "Completed $iterations iterations. cn = $cn"

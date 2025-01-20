#!/bin/bash

# This is a comment, explaining what the script does
# Print "Hello, World!" to the terminal
echo "Hello, World!"
# sleep 5

# Check if there are any arguments passed to the script
if [ "$#" -gt 0 ]; then
    echo "Arguments passed to the script:"
    for arg in "$@"; do
        echo "$arg"
    done
else
    echo "No arguments were passed to the script."
fi

# List the contents of the current directory
ls -l

# while true; do
#     echo "Running infinite loop..."
#     sleep 1
# done

# while [ true ]; do
#     echo "boucle"
# done


# End of script

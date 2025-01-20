#!/bin/bash

echo "Hello, World!"

if [ "$#" -gt 0 ]; then
    echo "Arguments passed to the script:"
    for arg in "$@"; do
        echo "$arg"
    done
else
    echo "No arguments were passed to the script."
fi

ls -l

# Infinite loop
while true; do
    echo "Running infinite loop..."
    sleep 1
done

# End of script

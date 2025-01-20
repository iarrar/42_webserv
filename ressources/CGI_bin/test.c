#include <stdio.h>

void printArguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
}

int main(int argc, char *argv[]) {
    printArguments(argc, argv);
    return 0;
}
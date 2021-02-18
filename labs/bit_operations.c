#include <stdio.h>

void bin_print(unsigned int i) {
    // Number of bits in an integer the compiler uses to store value
    int j = sizeof(unsigned int) * 8;

    // temp var
    int k;

    // Loop over the amount of bits in i
    for (j--; j >= 0; j--)
    {
        // ternary operator, if the condition is true, return 1,
        // otherwise return 0.
        k = ((1 << j) & i) ? 1 : 0;
        printf("%d", k);
    }
    printf(" ");
}

// main
int main(int argc, char * argv[]) {

    unsigned int i = 0xffffffff; // 2^32-1 or 4294967295, max value an int can hold

    // Number of bytes the compiler uses to store the value
    printf("Byte size of int: %lu\n", sizeof(i));
    printf("%d\n", i);

    printf("Original:  ");
    
    // Print i in binary
    bin_print(i);
    printf("Hex: \t%x\tDec:  %u\n\n", i, i);

    
    for (int j; j < 40; j++)
    {
        // Prints the operation
        // i.e the value (i) shifted j positions
        printf("%3d << %2d: ", i, j);

        // bits get shifted to the left, then destroyed going towards the left
        bin_print(i << j);

        // bits get shifted to the right, then destroyed going towards the right
        bin_print(i >> j);
        printf(": %3d >> %2d", i, j);
        printf("\n");
    }

    return 0;
}
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
        // Pick out the j^th bit of i
        k = ((1 << j) & i) ? 1 : 0;
        printf("%d", k);
    }
    printf(" ");
}

// main
int main(int argc, char *argv[]) {

    unsigned int i = 0x0f0f0f0f; // 2^32-1 or 4294967295, max value an int can hold

    // Number of bytes the compiler uses to store the value
    printf("Byte size of int: %lu\n", sizeof(i));
    
    // Will return -1 because int (4 bytes) is declared as unsigned(5 bytes), change to %u to see value
    printf("Value, displayed using %%d: %d\n", i);
    printf("Value, displayed using %%u: %u\n", i);

    printf("\nBin\t: ");
    // Print i in binary
    bin_print(i);
    printf("\nHex\t: %x\tDec:  %u\n\n", i, i);

    // 32
    int j = sizeof(unsigned int) * 8;

    for (j--; j >= 0; j--)
    {
        // 1 shifted left j times
        bin_print(1 << j); printf("\n");
        // i
        bin_print(i); printf("\n");
        printf("-------------------------------- &\n");

        // (1 shifted left j times) bitwise logical and i
        bin_print(((1 << j) & i));

        if (((1 << j) & i) == 0)
        {
            printf(" - BITMASK\n");
        }
        

        printf("\n");
    }

    return 0;
}
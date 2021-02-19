#include <stdio.h>
#include <stdint.h>

// https://stackoverflow.com/questions/9606455/how-to-specify-64-bit-integers-in-c
__int64_t rotr_x, rotl_x;
__int64_t i64 = 0x921FFAB1234C1DFFLL; // random 64 bit int

void bin_print(unsigned int i) {
    // Number of bits in an integer the compiler uses to store value
    int j = sizeof(unsigned int) * 16;

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

// Referenced and adapted from: https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
// n is the number of rotations
// d is the value passed
// w is the number of bits in d
void rotateRight(int n, __int64_t d) {
    unsigned int w = sizeof(d) * 8;

    // loops around n times
    for (int i = 0; i < n; i++)
    {
        // formula from NIST paper on Secure Hash Algorithms
        // when rotating bits to the right, we shift d by i (n times)
        // and use the bitwise OR on (i << (w - n)) to bring the bits to the back
        rotr_x = (d >> i) | (i << (w - n));
        // print the binary
        bin_print(rotr_x);
        printf("\n");
    }
}

// Referenced and adapted from: https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
// n is the number of rotations
// d is the value passed
// w is the number of bits in d
void rotateLeft(int n, __int64_t d) {

    unsigned int w = sizeof(d) * 8;

    // loops around n times
    for (int i = 0; i < n; i++)
    {
        // formula from NIST paper on Secure Hash Algorithms
        // when rotating bits to the left, we shift d by i (n times)
        // and use the bitwise OR on (i >> (w - n)) to bring the bits
        // to the front
        rotl_x = (d << i) | (i >> (w - n));
        // print the binary
        bin_print(rotl_x);
        printf("\n");
    }
}


// main
int main(int argc, char *argv[]) {

    unsigned int i = 0x0f0f0f0f;

    int rotationOf = 10;
    int rotateBy = 10;

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
        printf("\n\n");
    }

    rotationOf = i64;

    printf("Right rotation of %d by %d: \n", rotationOf, rotateBy);
    rotateRight(rotateBy, rotationOf);

    printf("Left rotation of %d by %d: \n", rotationOf, rotateBy);
    rotateLeft(rotateBy, rotationOf);

    return 0;
}
#include <stdio.h>
#include <inttypes.h>

#define BYTE uint8_t

void bin_print(BYTE i) {
    // Number of bits in an integer the compiler uses to store value
    int j = sizeof(BYTE) * 8;

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

int main(int argc, char *argv[]) {
    BYTE b;
    
    FILE *f;
    
    f = fopen(argv[1], "r");

    size_t nobytes;

    // read a byte from a file
    nobytes = fread(&b, 1, 1, f);

    while(nobytes) {
        bin_print(b);
        nobytes = fread(&b, 1, 1, f);
    }

    fclose(f);
    printf("\n");

    return 0;
}
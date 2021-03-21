#include <stdio.h>
#include <inttypes.h>

#define W 64
#define WORD uint64_t
#define PFDEC PRIu64
#define PFHEX PRIX64
#define BYTE uint8_t

union Block {
    BYTE bytes[64];
    WORD words[16];
};


int main(int argc, char *argv[]) {
    
    union Block B;
    int i;
    // Number of bits read
    uint64_t nobits = 0;

    // File pointer    
    FILE *f;

    // Open file from command line
    f = fopen(argv[1], "r");

    // Number of bytes read
    size_t nobytes;

    // Try to read 64 bytes
    nobytes = fread(B.bytes, 1, 64, f);
    printf("Read %ld bytes \n", nobytes);

    // Update the amount of bits read in.
    nobits = nobits + (8 * nobytes);

    for (i = 0; i < 16; i++){
        printf("%08" PFDEC " ", B.words[i]);

        if((i + 1) % 8 == 0)
            printf("\n");
    }
    
    while(!feof(f)) {
        // Try to read 64 bytes
        nobytes = fread(B.bytes, 1, 64, f);
        printf("Read %ld bytes \n", nobytes);
        nobits = nobits + (8 * nobytes);

        for (i = 0; i < 16; i++){
            printf("%08" PFDEC " ", B.words[i]);

            if((i + 1) % 8 == 0)
                printf("\n");
        }
    }
    // Close the file
    fclose(f);
    // Print the total number of bits
    printf("Total bits read: %ld\n", nobits);

    return 0;
}
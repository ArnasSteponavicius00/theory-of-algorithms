#include <stdio.h>
#include <inttypes.h>

#define W 64
#define WORD uint64_t
#define PFDEC PRIu64
#define PFHEX PRIX64
#define BYTE uint16_t
#define BYTES_TO_READ 128

union Block {
    BYTE bytes[128];
    WORD words[32];
    uint64_t sixf[16];
};

enum Status {
    READ, PAD, END
};

// Get the next block
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits) {
    
    // Number of bytes read
    size_t nobytes;


    if (*S == END) {
        return 0;
    } if(*S == READ) {
        // Try to read 128 bytes
        nobytes = fread(B->bytes, 1, BYTES_TO_READ, f);
        // update the total bits read
        *nobits = *nobits + (16 * nobytes);
        
        // Try to read 128 bytes, if number of bytes read in has less 
        // than 111 bytes, appending padding to the current block
        // 128 - 16 - 1 = 111
        // Enough room for padding
        if (nobytes == 128) {
            return 1;
        } else if (nobytes < 111) {
            
            B->bytes[nobytes++] = 0x8000; // Bits: 1000000000000000
            
            while (nobytes++ < 112) {
                B->bytes[nobytes] = 0x00;
            }            
            
            // Append length of original input (Check Endian)
            B->sixf[15] = *nobits;
            
            // broadcast this is the last block
            *S = END;
        } else {
            // Got to the end of the input message.
            // not enough room in this block for all the padding
            // Append a 1 bit (and seven 0 bits to make a full byte)
            B->bytes[nobytes] = 0x8000;
            // Append 0 bits
            while (nobytes++ < BYTES_TO_READ) {
                B->bytes[nobytes] = 0x00;
            }
            // Change the status to PAD
            *S = PAD;
        }
        
    } else if (*S == PAD) {
        nobytes = 0;
        B->bytes[nobytes] = 0x8000;
        // Append 0 bits
        while (nobytes++ < BYTES_TO_READ) {
            B->bytes[nobytes] = 0x00;
        }

        // Append nobits as an integer
        B->sixf[15] = *nobits;
        *S = PAD;
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    
    union Block B;
    int i;
    // Number of bits read
    uint64_t nobits = 0;

    // current Status when reading input
    enum Status S = READ;

    // File pointer    
    FILE *f;

    size_t nobytes;

    // Open file from command line
    f = fopen(argv[1], "r");

    // loop through the preprocessend blocks
    while(next_block(f, &B, &S, &nobits)) {
        for (i = 0; i < 32; i++){
            printf("%08" PFHEX " ", B.words[i]);
        }
        printf("\n\n");
    }
    // Close the file
    fclose(f);
    // Print the total number of bits
    printf("Total bits read: %ld\n", nobits);

    return 0;
}
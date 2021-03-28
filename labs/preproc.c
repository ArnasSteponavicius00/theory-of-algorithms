#include <stdio.h>
#include <inttypes.h>

#define W 64
#define WORD uint64_t
#define PFDEC PRIu64
#define PFHEX PRIX64
#define BYTE uint16_t
#define BYTES_TO_READ 128

// SHA works on block of 1024 bits
union Block {
    // 8 x 128 = 1024 - deals with blocks as bytes
    BYTE bytes[128];
    // 64 x 16 = 1024 - deals with blocks as words - 64 bit words
    WORD words[16];
    // 128 x 8 = 1024 - deals with the last 128 bits of the final block
    uint64_t sixf[8];
};

// Keep track of where we are with the input message/padding
enum Status {
    READ, PAD, END
};

// Returns 1 if a new block is created from the original message
// Returns 0 if all of the padded mesage has been consumed
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
        // than 111 bytes, append padding to the current block
        // 128 - 16 - 1 = 111
        // Enough room for padding
        if (nobytes == 128) {
            return 1;
        } else if (nobytes < 112) {
            // Append 1 bit and fifteen 0 bits
            B->bytes[nobytes++] = 0x8000; // Bits: 1000000000000000
            
            // Append bits leaving 128 at the end
            for (nobytes++; nobytes++ < 112; nobytes++) {
                B->bytes[nobytes] = 0x00;
            }            
            
            // Append length of original input (Check Endian)
            B->sixf[15] = *nobits;
            
            // broadcast this is the last block
            *S = END;
        } else {
            // Got to the end of the input message.
            // not enough room in this block for all the padding
            // Append a 1 bit and fifteen 0 bits
            B->bytes[nobytes++] = 0x8000;
            // Append 0 bits
            for (nobytes++; nobytes++ < BYTES_TO_READ; nobytes++) {
                // Error: was trying to write to B->nobytes[128]
                B->bytes[nobytes] = 0x00;
            }
            // Change the status to PAD
            *S = PAD;
        }
        
    } else if (*S == PAD) {
        nobytes = 0;
        // Append 0 bits
        for (nobytes = 0; nobytes < 112; nobytes++) {
            B->bytes[nobytes] = 0x00;
        }
        // Append nobits as an integer
        B->sixf[15] = *nobits;
        // Change the status
        *S = END;
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

    // loop through the preprocess end blocks
    while(next_block(f, &B, &S, &nobits)) {
        for (i = 0; i < 32; i++){
            printf("%016" PFHEX " ", B.words[i]);
        }
        printf("\n");
    }
    // Close the file
    fclose(f);
    // Print the total number of bits
    printf("Total bits read: %ld\n", nobits);

    return 0;
}
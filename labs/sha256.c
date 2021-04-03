#include <stdio.h>
#include <inttypes.h>
#include <byteswap.h>

// Endianess
// ref: https://developer.ibm.com/technologies/systems/articles/au-endianc/
const int _i = 1;
#define islilend() ((*(char*)&_i) != 0)

#define WLEN 32
#define WORD uint32_t
#define PFDEC PRIu64
#define PFHEX PRIx32
#define BYTE uint8_t
#define BYTES_TO_READ 64

// Page 5 of Secure Hash Standard
#define ROTL(x,n) ((x << n) | (x >> (WLEN - n)))
#define ROTR(x,n) ((x >> n) | (x << (WLEN - n)))
#define SHR(x,n) (x >> n)

// Page 10 of the secure hash standard.
#define Ch(x,_y,_z) ((x & _y) ^ (~x & _z))
#define Maj(x,_y,_z) ((x & _y) ^ (x & _z) ^ (_y & _z))

#define SIG0(x) (ROTR(x,2)  ^ ROTR(x,13) ^ ROTR(x,22))
#define SIG1(x) (ROTR(x,6)  ^ ROTR(x,11) ^ ROTR(x,25))
#define Sig0(x) (ROTR(x,7)  ^ ROTR(x,18) ^ SHR(x,3))
#define Sig1(x) (ROTR(x,17) ^ ROTR(x,19) ^ SHR(x,10))

// SHA works on block of 512 bits
union Block {
    // 8 x 64 = 512 - deals with blocks as bytes
    BYTE bytes[64];
    // 16 x 16 = 512 - deals with blocks as words - 64 bit words
    WORD words[16];
    // 64 x 8 = 512 - deals with the last 64 bits of the final block
    uint64_t sixf[8];
};

// Keep track of where we are with the input message/padding
enum Status {
    READ, PAD, END
};

// Page 17 Secure Hash Standard Constants
const WORD K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Returns 1 if a new block is created from the original message
// Returns 0 if all of the padded mesage has been consumed
int next_block(FILE *f, union Block *M, enum Status *S, uint64_t *nobits) {
    
    // Number of bytes read
    size_t nobytes;

    if (*S == END) {
        return 0;
    } else if (*S == READ) {
        // Try to read 64 bytes
        nobytes = fread(M->bytes, 1, BYTES_TO_READ, f);
        // update the total bits read
        *nobits = *nobits + (8 * nobytes);

        if (nobytes == 64) {
            // Do nothing
        } else if (nobytes < 56) {
            // Append 1 bit and seven 0 bits
            M->bytes[nobytes] = 0x80;
            
            // Append bits leaving 64 at the end
            for (nobytes++; nobytes < 56; nobytes++) {
                M->bytes[nobytes] = 0x00;
            }            
            
            // Append length of original input (Check Endian)
            M->sixf[7] = (islilend() ? bswap_64(*nobits) : *nobits);
            
            // broadcast this is the last block
            *S = END;
        } else {
            // Got to the end of the input message.
            // not enough room in this block for all the padding
            // Append a 1 bit and seven 0 bits
            M->bytes[nobytes] = 0x80;
            // Append 0 bits
            for (nobytes++; nobytes++ < BYTES_TO_READ; nobytes++) {
                M->bytes[nobytes] = 0x00;
            }
            // Change the status to PAD
            *S = PAD;
        }
    } else if (*S == PAD) {
        // Append 0 bits
        for (nobytes = 0; nobytes < 56; nobytes++) {
            M->bytes[nobytes] = 0x00;
        }
        // Append nobits as an integer
        M->sixf[7] = (islilend() ? bswap_64(*nobits) : *nobits);
        // Change the status
        *S = END;
    }

    // Swap byte order of the words if machine is little endian
    if (islilend())
        for (int i = 0; i < 16; i++)
            M->words[i] = bswap_32(M->words[i]);
    
    return 1;
}

int next_hash(union Block *M, WORD H[]) {
    // Message schedule
    WORD W[64];
    // Iterator for w
    int t;

    // Temp vars
    WORD a, b, c, d, e, f, g, h, T1, T2;

    // Section 6.2.2, pt 1
    for (t = 0; t < 16; t++)
        W[t] = M->words[t];

    for (t = 16; t < 64; t++)
        W[t] = Sig1(W[t-2]) + W[t-7] + Sig0(W[t-15]) + W[t-16];

    // Section 6.2.2, pt 2
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    // Section 6.2.2, pt 3
    for (t = 0; t < 64; t++)
    {
        T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = SIG0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }    

    // Section 6.2.2, pt 4
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[7] = h + H[7];

    return 0;
}

// The function that performs the sha356 algortihm on the message
int sha256(FILE *f, WORD H[]) {
    // current  message block
    union Block M;
    // Number of bits read
    uint64_t nobits = 0;

    // current Status when reading input
    enum Status S = READ;
    
    // loop through the preprocess end blocks
    while(next_block(f, &M, &S, &nobits)) {
        next_hash(&M, H);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // Initial hash values
    WORD H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // File pointer    
    FILE *f;


    // Open file from command line
    f = fopen(argv[1], "r");

    // calculate sha256 of f
    sha256(f, H);

    for (int i = 0; i < 8; i++) {
        printf("%08" PFHEX, H[i]);
    }
    printf("\n");

    // Close the file
    fclose(f);

    return 0;
}
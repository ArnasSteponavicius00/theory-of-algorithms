#include <stdio.h>
#include <inttypes.h>

#define WORD uint64_t
#define PFDEC PRIu64
#define PFHEX PRIX64

#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

//Original code before making the functions into pre-processors
/* (x & y) = chooses where the bits are set to 1 in x, the corresponding y WORDue will be picked
 if y is 0 in the pos of x , output will be 0, if y is 1 in that pos output will be 1.
 (~x & z) = output will be like a merging of y and z, i.e where x is 1, bits will come from y
 where x is 0, bits will come from z 

 WORD Ch(WORD x, WORD y, WORD z) {
     return (x & y) ^ (~x & z);
 }

 WORD Maj(WORD x, WORD y, WORD z) {
     return (x & y) ^ (x & z) ^ (y & z);
 }
 */

int main(int argc, char *argv[]) {

    WORD x = 0x0F0F0F0F0F0F0F0FLL;
    WORD y = 0x0A0A0A0A0A0A0A0ALL;
    WORD z = 0x0B0B0B0B0B0B0B0BLL;

    WORD ansCh = Ch(x, y, z);
    WORD ansMaj = Ch(x, y, z);

    printf("\nCh function\nDec - Ch(%" PFDEC ",%" PFDEC ",%" PFDEC ")=%" PFDEC "\n", x, y, z, ansCh);
    printf("Hex - Ch(%" PFHEX ",%" PFHEX ",%" PFHEX ")=%" PFHEX "\n", x, y, z, ansCh);
    
    printf("\nMaj function\n");

    printf("Dec - Maj(%" PFDEC ",%" PFDEC ",%" PFDEC ")=%" PFDEC "\n", x, y, z, ansMaj);
    printf("Hex - Maj(%" PFHEX ",%" PFHEX ",%" PFHEX ")=%" PFHEX "\n", x, y, z, ansMaj);

    return 0;
}
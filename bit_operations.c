#include <stdio.h>

void bin_print(int i) {
    // Number of bits in an integer
    int j = sizeof(int) * 8 - 1;

    int k;

    for (j--; j >= 0; j--)
    {
        k = ((1 << j) & i) ? 1 : 0;
        printf("%d", k);
    }
    
}

int main() {
    int i = 241;    

    printf("%ld\n", sizeof(i));
    printf("\n");

    bin_print(i);

    //printf("%d\n", sizeof(int));
    


    for (int j; j < 40; j++)
    {
        printf("%3d << %2d: ", i, j);
        bin_print(i << j);
        printf("\n");
    }
    

    return 0;
}
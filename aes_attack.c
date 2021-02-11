#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 16
#define SET_SIZE 256

int main () {

    unsigned char template[BLOCK_SIZE];
    unsigned char DeltaSetContainer[SET_SIZE][BLOCK_SIZE]; // = {0};

    //srand(time(NULL)); // Seed (for generating the delta sets)
    for(int i = 1; i < BLOCK_SIZE; i++) {
        template[i] = rand() % (0xFF + 1);
    }
    
    for(int i = 0; i < SET_SIZE; i++){
        memcpy(DeltaSetContainer[i], template, BLOCK_SIZE);
        DeltaSetContainer[i][0] = (unsigned char) i;
    }
    
    for (int i = 0; i < 16; i++) {
        printf("%02x ", DeltaSetContainer[3][i]);
        printf("%02x ", template[i]);
        printf("\n");
    }

    return 0;
}


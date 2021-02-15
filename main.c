#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "aes.h"
#include "key_schedule.h"

unsigned char ReverseLastRound (unsigned char data_byte, unsigned char last_key_byte_guess) {
    data_byte ^= last_key_byte_guess;
    data_byte = SI[data_byte];

    return data_byte;
}

int CheckGuess (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], int byte_position, unsigned char key_byte_guess) {

    unsigned char byte_sum = 0;
    
    for (int i = 0; i < SET_SIZE; i++) {
        byte_sum ^= ReverseLastRound(lambda_set[i][byte_position], key_byte_guess);
    }

    if (byte_sum == 00) {
        return 1;
    } else {
        return 0;
    }
}

/*
unsigned char** CheckGuesses (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], unsigned char* result[BLOCK_SIZE][SET_SIZE]) {

    //unsigned char* byte_sums[BLOCK_SIZE];

    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < SET_SIZE; j++) {
            result[i][j] = CheckGuess (lambda_set, i, (unsigned char)j);
        }
    }

    return result;
}
*/

unsigned char* SquareAttack(unsigned char RoundKey[]){

    // We define the key and a copy for moving data around
    unsigned char key[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
    };

    unsigned char key_original[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
    };

     int result_temp[BLOCK_SIZE][SET_SIZE];
     for (int k=0; k<BLOCK_SIZE; k++){
            for (int i = 0; i < SET_SIZE; i++) {
                result_temp[k][i]=1; 
            
            }
        }
    
    //We create a Lambda set and encrypt is using 4 rounds of AES
    int NoFoundBytes=0;
    
    while(NoFoundBytes<16){
        NoFoundBytes=0;
        
        unsigned char template[BLOCK_SIZE];
        unsigned char DeltaSetContainer[SET_SIZE][BLOCK_SIZE]; // = {0};

        for(int i = 1; i < BLOCK_SIZE; i++) {
                template[i] = rand() % (0xFF + 1);
        }

        for(int i = 0; i < SET_SIZE; i++){
            memcpy(DeltaSetContainer[i], template, BLOCK_SIZE);
            DeltaSetContainer[i][0] = (unsigned char) i;
            AES4Rounds(DeltaSetContainer[i], key);
            for (int j = 0; j < BLOCK_SIZE; j++) {
                key[j] = key_original[j];
                }
            }
        
        unsigned char result[BLOCK_SIZE][SET_SIZE];
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < SET_SIZE; j++) {
                result[i][j] = CheckGuess (DeltaSetContainer, i, (unsigned char)j);
            }
        }

        for (int k=0; k<BLOCK_SIZE; k++){
            int count=0;
            int x=0;
            for (int i = 0; i < SET_SIZE; i++) {
                if (result[k][i] == 1 && result_temp[k][i]==1) {
                    x=i;
                    count+=1;
                }
                }
            if (count==1){
                RoundKey[k]=x;
                NoFoundBytes+=1;
            
            }
        }
        
        for (int k=0; k<BLOCK_SIZE; k++){
            for (int i = 0; i < SET_SIZE; i++) {
                result_temp[k][i]=result[k][i];
            
            }
        }
        
    }

    return RoundKey; 
}

int main () {
    unsigned char key_test[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    unsigned char key[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
    };

    unsigned char key_original[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
    };

    unsigned char template[BLOCK_SIZE];
    unsigned char DeltaSetContainer[SET_SIZE][BLOCK_SIZE]; // = {0};

    //srand(time(NULL)); // Seed (for generating the delta sets)
    for(int i = 1; i < BLOCK_SIZE; i++) {
        template[i] = rand() % (0xFF + 1);
    }
    
    for(int i = 0; i < SET_SIZE; i++){
        memcpy(DeltaSetContainer[i], template, BLOCK_SIZE);
        DeltaSetContainer[i][0] = (unsigned char) i;

        AES4Rounds(DeltaSetContainer[i], key);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            key[j] = key_original[j];
        }

    }
    
    unsigned char xor_result[BLOCK_SIZE] =
    {0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0};
    
    for (int i = 0; i < SET_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            xor_result[j] ^= DeltaSetContainer[i][j];
        }
    }
    
    
    unsigned char result[BLOCK_SIZE][SET_SIZE];
    
    for (int i = 0; i < BLOCK_SIZE; i++) {
        for (int j = 0; j < SET_SIZE; j++) {
            result[i][j] = CheckGuess (DeltaSetContainer, i, (unsigned char)j);
        }
    }

    SquareAttack(key_test);

    for (int i = 0; i < 16; i++) {
        printf("%02x ", key_test[i]);
    }

    KeyScheduleInverse(key_test, 4);

    printf("\n");

    for (int i = 0; i < 16; i++) {
        printf("%02x ", key_test[i]);
    }

    return 0;
}


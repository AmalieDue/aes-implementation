#include "defines.h"
#include "aes.h"
#include "square_attack.h"

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

unsigned char* SquareAttack(AES* aes){

    // We define the key and a copy for moving data around
    
    unsigned char fourth_round_key[BLOCK_SIZE];
    unsigned char key_original[BLOCK_SIZE];
    memcpy(key_original, aes->key, BLOCK_SIZE);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        key_original[i] = aes->key[i];
    }
    
    int correct_key_bytes[BLOCK_SIZE][SET_SIZE];
    int current_correct_key_bytes[BLOCK_SIZE][SET_SIZE];
    
    for (int k = 0; k<BLOCK_SIZE; k++){
        for (int i = 0; i < SET_SIZE; i++) {
            correct_key_bytes[k][i] = 1; 
        }
    }
    
    // We create as many Lambda sets as it takes to find all bytes
    // of the last round key, using 4 rounds of AES encryption
    
    int NoFoundBytes = 0;
    
    while(NoFoundBytes < BLOCK_SIZE){
        NoFoundBytes = 0;
        
        unsigned char template[BLOCK_SIZE];
        unsigned char DeltaSetContainer[SET_SIZE][BLOCK_SIZE];

        for(int i = 1; i < BLOCK_SIZE; i++) {
            template[i] = rand() % (0xFF + 1);
        }

        for(int i = 0; i < SET_SIZE; i++){
            memcpy(DeltaSetContainer[i], template, BLOCK_SIZE);
            DeltaSetContainer[i][0] = (unsigned char) i;

            AES4Rounds(aes, DeltaSetContainer[i]);

            for (int i = 0; i < BLOCK_SIZE; i++) {
                aes->key[i] = key_original[i];
            }
        }

        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < SET_SIZE; j++) {
                current_correct_key_bytes[i][j] = CheckGuess (DeltaSetContainer, i, (unsigned char)j);
            }
        }

        for (int i = 0; i < BLOCK_SIZE; i++){
            int count_matching_bytes = 0;
            int x = 0;
            for (int j = 0; j < SET_SIZE; j++) {
                if (current_correct_key_bytes[i][j] == 1 && correct_key_bytes[i][j] == 1) {
                    x = j;
                    count_matching_bytes += 1;
                }
            }
            if (count_matching_bytes == 1){
                fourth_round_key[i] = x;
                NoFoundBytes += 1;
            }
        }
    
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < SET_SIZE; j++) {
                correct_key_bytes[i][j] = current_correct_key_bytes[i][j];
            }
        }
    }

    memcpy(aes->key, fourth_round_key, BLOCK_SIZE);

    return aes->key; 
}

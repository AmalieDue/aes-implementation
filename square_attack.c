//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "aes.h"
#include "square_attack.h"



/*
*Desctiption: Function that reverses the last round of AES which is used for the square attack using a possible round key byte. 
*Shift rows is not considered since each byte is considered individually
*
*Input: A data byte from the AES encryption and a possible corrosponding round key byte
* 
*Output: The data_byte after the decryption of the last AES round:
*/
unsigned char decrypt_round_one_byte (unsigned char data_byte, unsigned char key_byte) {
    data_byte ^= key_byte;
    data_byte = SI[data_byte];

    return data_byte;
}

/*
*Desctiption: Function that checks if the guess of a certain key byte is a pssible candidate for the round key byte or not. 
*
*Input: A lambda-set containing a number of ciphertext, a byte position and the guess of the round key byte at that position
* 
*Output: 1 if the guess is valid, 0 if the guess is wrong. 
*/
int check_key_byte_guess (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], int byte_position, unsigned char key_byte_guess) {

    unsigned char byte_sum = 0;
    
    for (int i = 0; i < SET_SIZE; i++) {
        byte_sum ^= decrypt_round_one_byte(lambda_set[i][byte_position], key_byte_guess);
    }

    if (byte_sum == 00) {
        return 1;
    } else {
        return 0;
    }
}

/*
*Desctiption: Function that performs the square attack on 4 rounds of AES. 
*
*Input: An AES-instance
* 
*Output: The 4'th round key. 
*/
unsigned char* square_attack(AES* aes){

    // We define the key and a copy for moving data around
    
    unsigned char fourth_round_key[BLOCK_SIZE]; //Array to input the correct round key bytes.
    unsigned char key_original[BLOCK_SIZE]; //Array containing the original AES key. 
    memcpy(key_original, aes->key, BLOCK_SIZE);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        key_original[i] = aes->key[i];
    }

    int found_byte_positions[BLOCK_SIZE]; //Array where an entry is 1 if the corrosponing key byte is found and 0 otherwise

    for (int i = 0; i < BLOCK_SIZE; i++) {
        found_byte_positions[i] = 0;
    }

    
    int correct_key_bytes[BLOCK_SIZE][SET_SIZE]; //2D-array for the current state of the attack. if entry (i,j)=1 j is a valid candidate for byte i n the roundkey. Otherwise (i,j)=0
    int current_correct_key_bytes[BLOCK_SIZE][SET_SIZE]; //2D-array for a certain lambda set. if entry (i,j)=1 j is a valid candidate for byte i n the roundkey. Otherwise (i,j)=0
    
    for (int k = 0; k<BLOCK_SIZE; k++){
        for (int i = 0; i < SET_SIZE; i++) {
            correct_key_bytes[k][i] = 1; 
        }
    }
    
    // We create as many Lambda sets as it takes to find all bytes
    // of the last round key, using 4 rounds of AES encryption
    
    int NoFoundBytes = 0;
    
    while(NoFoundBytes < BLOCK_SIZE){ //We keep performing the attack until all 16 bytes of the round key is found. 
        NoFoundBytes = 0;
        
        //First we create a random lambda-set
        unsigned char template[BLOCK_SIZE];
        unsigned char DeltaSetContainer[SET_SIZE][BLOCK_SIZE];

        for(int i = 1; i < BLOCK_SIZE; i++) {
            template[i] = rand() % (0xFF + 1);
        }

        for(int i = 0; i < SET_SIZE; i++){
            memcpy(DeltaSetContainer[i], template, BLOCK_SIZE);
            DeltaSetContainer[i][0] = (unsigned char) i;

            aes_encryption_4_rounds(aes, DeltaSetContainer[i]);

            for (int i = 0; i < BLOCK_SIZE; i++) {
                aes->key[i] = key_original[i];
            }
        }

        //We then find the possible  candidates for each og the 16 bytes of the roundkey
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < SET_SIZE; j++) {
                current_correct_key_bytes[i][j] = check_key_byte_guess (DeltaSetContainer, i, (unsigned char)j);
            }
        }

        //We compare the possible canidates to the candidates already found in previous rounds. 
        for (int i = 0; i < BLOCK_SIZE; i++){
            int count_matching_bytes = 0;
            int x = 0;
            if (found_byte_positions[i] == 1) { //If a byte in the roundkey is already found we dont find it again. 
                NoFoundBytes++;
            } else if (found_byte_positions[i] == 0) {
                for (int j = 0; j < SET_SIZE; j++) { //We count the possible candidates for each roundkey byte
                    if (current_correct_key_bytes[i][j] == 1 && correct_key_bytes[i][j] == 1) {
                        x = j;
                        count_matching_bytes += 1;
                    }
                }
                if (count_matching_bytes == 1){ //If there is only a single candidate for the roundkey byte we will insert it into the roundkey adn mark this byte position as found
                    fourth_round_key[i] = x;
                    found_byte_positions[i] = 1;
                    NoFoundBytes += 1;
                }
            }
        }

        //We then update the currencty possible candidates for each byte position.
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < SET_SIZE; j++) {
                correct_key_bytes[i][j] = current_correct_key_bytes[i][j];
            }
        }
    }

    memcpy(aes->key, fourth_round_key, BLOCK_SIZE);
    //We return the found round key 
    return aes->key; 
}

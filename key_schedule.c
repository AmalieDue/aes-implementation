//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#include <string.h>
#include "defines.h"
#include "aes.h"
#include "key_schedule.h"

/*
* Description: Key Schedule algorithm. Calculates the next round key.
*
* Input: Round key from previous round and the current round number. I.e. if
* we are doing fourth round of encryption, then the inputs are the round key
* from third round and the integer round = 4
*
* Output: The next round key
*/
unsigned char * key_schedule (unsigned char key_in[], int round) {
    unsigned char key_out[BLOCK_SIZE];

    unsigned char Rcon[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    
    key_out[0] = S[key_in[7]] ^ key_in[0] ^ Rcon[round-1];
    key_out[4] = S[key_in[11]] ^ key_in[4];
    key_out[8] = S[key_in[15]] ^ key_in[8];
    key_out[12] = S[key_in[3]] ^ key_in[12];

    for (int i = 1; i < 4; i++) {
        key_out[i] = key_out[i-1] ^ key_in[i];
    }

    for (int i = 5; i < 8; i++) {
        key_out[i] = key_out[i-1] ^ key_in[i];
    }

    for (int i = 9; i < 12; i++) {
        key_out[i] = key_out[i-1] ^ key_in[i];
    }

    for (int i = 13; i < 16; i++) {
        key_out[i] = key_out[i-1] ^ key_in[i];
    }

    memcpy(key_in, key_out, BLOCK_SIZE);

    return key_in;
}

/*
* Description: Inverse Key Schedule algorithm. The master key is derived from
* some input round key.
* 
* Input: Some round key and the round number corresponding to the round key. I.e. if
* the fourth round key is given, then also the integer round = 4 is given.
*
* Output: The master key derived from the input round key
*/
unsigned char * derive_master_key(unsigned char key_in[], int round) {
    unsigned char key_out[BLOCK_SIZE];

    unsigned char Rcon[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

    for (int i = round; i > 0; i--) {

        for (int i = 1; i < 4; i++) {
            key_out[i] = key_in[i-1] ^ key_in[i];
        }

        for (int i = 5; i < 8; i++) {
            key_out[i] = key_in[i-1] ^ key_in[i];
        }

        for (int i = 9; i < 12; i++) {
            key_out[i] = key_in[i-1] ^ key_in[i];
        }

        for (int i = 13; i < 16; i++) {
            key_out[i] = key_in[i-1] ^ key_in[i];
        }

        key_out[0] = S[key_out[7]] ^ key_in[0] ^ Rcon[i-1];
        key_out[4] = S[key_out[11]] ^ key_in[4];
        key_out[8] = S[key_out[15]] ^ key_in[8];
        key_out[12] = S[key_out[3]] ^ key_in[12];
        
        memcpy(key_in, key_out, BLOCK_SIZE);
    }

    return key_in;
}

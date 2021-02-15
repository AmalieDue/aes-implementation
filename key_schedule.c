//
// Created by bla bla
//

#include <string.h>
#include "defines.h"
#include "aes.h"
#include "key_schedule.h"

unsigned char * KeySchedule (unsigned char key_in[], int round) {
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

unsigned char * KeyScheduleInverse(unsigned char key_in[], int round) {
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

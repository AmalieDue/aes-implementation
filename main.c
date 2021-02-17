#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "aes.h"
#include "key_schedule.h"



int main () {
    
    unsigned char data[] = {
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

    AES* aes = create_aes_instance(key);

    //AES4Rounds(aes, data);

    SquareAttack(aes);

    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", aes->key[i]);
    }

    delete_aes_instance(aes);

    return 0;
}


//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "defines.h"
#include "aes.h"
#include "key_schedule.h"
#include "square_attack.h"


int main () {

    /*
    * Example one:
    * 
    * Given the master key: 2b28ab097eaef7cf15d2154f16a6883c,
    * we want to illustrate the following:
    * 
    * We apply square attack and derive the fourth round key:
    * efa8b6db4452710ba55b25ad417f3b00 
    * 
    * Applying the inverse Key Schedule algorithm, we derive the master key.
    */

    printf("\nEXAMPLE 1:");
    unsigned char key[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
    };

    printf("\nThe following master key is given:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", key[i]);
    }

    AES* aes = create_aes_instance(key);

    square_attack(aes);

    printf("\nThe square attack has derived the fourth round key, it can be seen below:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", aes->key[i]);
    }
    
    derive_master_key(aes->key, 4);

    printf("\nWe derive the master key from the fourth round key:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", aes->key[i]);
    }
    printf("\nSuccess! The correct master key has been derived.\n");

    delete_aes_instance(aes);

    /*
    * Example two:
    * 
    * We compute ten different master keys and apply the square attack.
    * 
    * For each test case, we test whether applying the inverse Key Schedule 
    * to the fourth round key results in the corresponding master key.
    * 
    * If success in all ten cases, we return "Success!", otherwise "Failure".
    */
    printf("\n\nEXAMPLE 2:");
    int number_of_tests = 10;

    unsigned char key_test[BLOCK_SIZE];

    //srand(time(NULL));
    for (int i = 0; i < number_of_tests; i++) {

        for(int k = 0; k < BLOCK_SIZE; k++) {
            key_test[k] = rand() % (0xFF + 1);
        }

        AES* aes = create_aes_instance(key_test);

        square_attack(aes);
        
        derive_master_key(aes->key, 4);

        int j = 0;
        while (j < BLOCK_SIZE + 1) {
            if (j == BLOCK_SIZE) {
                if (i == number_of_tests - 1) {
                    printf("\nSuccess! We managed to find the correct keys in %d examples\n", number_of_tests);
                }
            } else if (aes->key[j] != key_test[j]) {
                fprintf(stderr, "Error: Square attack did not break AES\n");
                fflush(stderr);
                return 1;
            }
            j++;
        }

        delete_aes_instance(aes);
    }

    /*
    * Example three:
    * 
    * We do a full AES encryption (10 rounds). Afterwards we decrypt and show that we have derived the plaintext.
    * 
    */

    printf("\n\nEXAMPLE 3:");
    aes = create_aes_instance(key);

    unsigned char data[] = {
    0x32, 0x88, 0x31, 0xe0,
    0x43, 0x5a, 0x31, 0x37,
    0xf6, 0x30, 0x98, 0x07,
    0xa8, 0x8d, 0xa2, 0x34
    };

    printf("\nPlaintext:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", data[i]);
    }

    aes_encryption_10_rounds(aes, data);

    printf("\nCiphertext:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", data[i]);
    }

    memcpy(aes->key, key, BLOCK_SIZE);
    
    aes_decryption_10_rounds(aes, data);

    printf("\nPlaintext:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", data[i]);
    }

    delete_aes_instance(aes);

    return 0;
}


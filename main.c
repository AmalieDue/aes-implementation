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

    unsigned char key[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
    };

    AES* aes = create_aes_instance(key);

    SquareAttack(aes);

    printf("Fourth round key:\n");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", aes->key[i]);
    }
    printf("\n");
    
    KeyScheduleInverse(aes->key, 4);

    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x ", aes->key[i]);
    }
    printf("\nSuccess! When applying the inverse key schedule, we arrive at the initial key\n");

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
    int number_of_tests = 10;

    unsigned char key_test[BLOCK_SIZE];

    srand(time(NULL));
    for (int i = 0; i < number_of_tests; i++) {

        for(int k = 0; k < BLOCK_SIZE; k++) {
            key_test[k] = rand() % (0xFF + 1);
        }

        AES* aes = create_aes_instance(key_test);

        SquareAttack(aes);
        
        KeyScheduleInverse(aes->key, 4);

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

    return 0;
}


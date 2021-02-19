//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#ifndef SQUAREATTACK_AES_H
#define SQUAREATTACK_AES_H

#include "aes.h"
#include "defines.h"

unsigned char decrypt_round_one_byte (unsigned char data_byte, unsigned char key_byte);

int check_key_byte_guess (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], int byte_position, unsigned char key_byte_guess);

unsigned char* square_attack(AES* aes);

#endif
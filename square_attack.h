//
// Created by bla bla
//

#include "aes.h"
#include "defines.h"

#ifndef SQUAREATTACK_AES_H
#define SQUAREATTACK_AES_H

unsigned char ReverseLastRound (unsigned char data_byte, unsigned char last_key_byte_guess);

int CheckGuess (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], int byte_position, unsigned char key_byte_guess);

unsigned char* SquareAttack(AES* aes);

#endif
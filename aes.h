//
// Created by bla bla
//

#ifndef AES_H
#define AES_H

typedef struct AES {
    size_t key_size;
    unsigned char* key;
} AES;

AES* create_aes_instance(unsigned char key[]);

void delete_aes_instance(AES* aes);

unsigned char * AES4Rounds(AES* aes, unsigned char data[]);

unsigned char S[256];

unsigned char SI[256];

#endif
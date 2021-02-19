//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#ifndef AES_H
#define AES_H

typedef struct AES {
    size_t key_size;
    unsigned char* key;
} AES;

AES* create_aes_instance(unsigned char key[]);

void delete_aes_instance(AES* aes);

unsigned char * aes_encryption_4_rounds(AES* aes, unsigned char data[]);

unsigned char * aes_encryption_10_rounds(AES* aes, unsigned char data[]);

unsigned char * aes_decryption_10_rounds(AES* aes, unsigned char data[]);

unsigned char S[256];

unsigned char SI[256];

#endif
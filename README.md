# Implementation of square attack on AES

> This repository contains our implementation of the square attack on AES in the course
02255 Modern Cryptology Spring 2021 at DTU. The group members are [Anders Lammert Hartmann](https://github.com/AndersHartmann) 
(Student ID: s153596) and [Amalie Due Jensen](https://github.com/AmalieDue) (Student ID: s160503).

## Structure

The project has been divided into the following files:

* `AES.c`: Contains all functions related to the AES encryption.
* `key_schedule.c`: Contains the key schedule implementation and the inverse key schedule implementation.
* `square_attack.c`: Contains functions used for doing a square attack.
* `main.c`: The main program.

## Usage

A simple example of encryption and decryption is shown below.

```c

// Define a master key
unsigned char key[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
};

// Define data that should be encrypted
unsigned char data[] = {
    0x32, 0x88, 0x31, 0xe0,
    0x43, 0x5a, 0x31, 0x37,
    0xf6, 0x30, 0x98, 0x07,
    0xa8, 0x8d, 0xa2, 0x34
};


// Create an AES instance with the given master key
AES* aes = create_aes_instance(key);

// Encrypt data
aes_encryption_10_rounds(aes, data);

// Decrypt data
aes_decryption_10_rounds(aes, data);

// We have decrypted data
printf("\nPlaintext:\n");
for (int i = 0; i < BLOCK_SIZE; i++) {
    printf("%02x ", data[i]);
}

```

Output:
```c
Plaintext:
32 88 31 e0 43 5a 31 37 f6 30 98 07 a8 8d a2 34
```

```c
// Success! We have encrypted and decrypted

delete_aes_instance(aes);
```

A simple example of how to do a square attack is shown below.

```c

// Define a master key
unsigned char key[] = {
    0x2b, 0x28, 0xab, 0x09,
    0x7e, 0xae, 0xf7, 0xcf,
    0x15, 0xd2, 0x15, 0x4f,
    0x16, 0xa6, 0x88, 0x3c
};

// Create an AES instance with the given master key
AES* aes = create_aes_instance(key);

// Perform square attack on the AES instance. Within this attack, the fourth round key is guessed 
//and returned by the function. The fourth round key is written into aes->key (it overwrites the 
//previous aes->key)
square_attack(aes);

// When printing out aes->key, we get the fourth round key which was guessed in the square attack
printf("Fourth round key:\n");
for (int i = 0; i < BLOCK_SIZE; i++) {
    printf("%02x ", aes->key[i]);
}
printf("\n");

```

Output:
```c
Fourth round key:
ef a8 b6 db 44 52 71 0b a5 5b 25 ad 41 7f 3b 00 
```

```c
// Applying the inverse Key Schedule algorithm, it is possible to derive the master key from the 
// fourth round key
derive_master_key(aes->key, 4);

for (int i = 0; i < BLOCK_SIZE; i++) {
    printf("%02x ", aes->key[i]);
}
```

Output:
```c
2b 28 ab 09 7e ae f7 cf 15 d2 15 4f 16 a6 88 3c 
```

```c
// Success! We have derived the master key

delete_aes_instance(aes);
```

## How to run the program (a Makefile is coming :-) )

Using the gcc compiler:

```c
gcc main.c aes.c key_schedule.c square_attack.c -o aes

./aes
```

## API

### Constants

* `BLOCK_SIZE = 16`:

Number of bytes in AES data (4 by 4 matrix of bytes).

* `SET_SIZE = 256`:

Number of plaintexts used in a square attack.

### AES functions

* `AES* create_aes_instance(unsigned char key[]);`:
Creates an AES instance, i.e. a struct with two properties, key and key_size. The function takes the master key as input.

* `void delete_aes_instance(AES* aes);`:
Delete the AES instance which was created. The function takes the AES instance as input.

* `unsigned char * sub_bytes (unsigned char data[]);`:
SubBytes operation of the AES encryption. The function takes the current AES data as input.

* `unsigned char * sub_bytes_inverse (unsigned char data[]);`:
Inverse SubBytes operation of the AES encryption. The function takes the current AES data as input.

* `unsigned char * shift_rows (unsigned char data[]);`:
ShiftRows operation of the AES encryption. The function takes the current AES data as input.

* `unsigned char * shift_rows_inverse (unsigned char data[]);`:
Inverse ShiftRows operation of the AES encryption. The function takes the current AES data as input.

* `unsigned char * mix_columns (unsigned char data[]);`:
MixColumns operation of the AES encryption. The function takes the current AES data as input.

* `unsigned char * mix_columns_inverse (unsigned char data[]);`:
Inverse MixColumns operation of the AES encryption. The function takes the current AES data as input.

* `unsigned char * add_round_key (unsigned char data[], unsigned char roundkey[]);`:
AddRoundKey operation of the AES encryption. The function takes the current AES data and the round key as inputs.

* `unsigned char * aes_encryption_4_rounds(AES* aes, unsigned char data[]);`:
Four rounds of AES encryption (for the square attack, we only want 4 rounds of AES encryption). The function takes the AES instance and the data that should be encrypted as inputs. 

* `unsigned char * aes_encryption_10_rounds(AES* aes, unsigned char data[]);`:
Ten rounds of AES encryption (the standard with this key size).

* `unsigned char * aes_decryption_10_rounds(AES* aes, unsigned char data[]);`:
Decryption of ten rounds of AES.

### Key Schedule functions

* `unsigned char * key_schedule (unsigned char key_in[], int round);`:
Key Schedule algorithm. The function takes the previous round key and the current round key as inputs and outputs the current round key.

* `unsigned char * derive_master_key(unsigned char key_in[], int round);`:
Inverse Key Schedule algorithm. The function takes some round key and the corresponding round number as inputs and derives and outputs and master key.

### Square attack functions

* `unsigned char decrypt_round_one_byte (unsigned char data_byte, unsigned char key_byte);`:
Decrypts one round in one AES byte. The function takes one byte of data and the corresponding byte from the round key as inputs.

* `int check_key_byte_guess (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], int byte_position, unsigned char key_byte_guess);`:
Checks whether a specific value is correct for a specific byte of the last round key. This is checked by decrypting the specific byte position in all ciphertexts in the lambda set
and then check whether the total XOR of that byte position throughout all ciphertexts in the lambda set sums to 0. More details are given in papers about the square attack.

* `unsigned char* square_attack(AES* aes);`:
Square attack. The function takes the AES instance as input and derives and outputs the last round key. Afterwards, the master key can be derived by passing the last round key 
to the `derive_master_key` function.
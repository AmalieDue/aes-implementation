# Implementation of square attack on AES

> This repository contains our implementation of the square attack on AES in the course
02255 Modern Cryptology Spring 2021 at DTU.

## Structure

## Usage

A simple example is shown below.

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
and returned by the function. The fourth round key is written into aes->key (it overwrites the 
previous aes->key)
SquareAttack(aes);

// When printing out aes->key, we get the fourth round key which was guessed in the square attack
printf("Fourth round key:\n");
for (int i = 0; i < BLOCK_SIZE; i++) {
    printf("%02x ", aes->key[i]);
}
printf("\n");

```

Output:
```c
ef a8 b6 db 44 52 71 0b a5 5b 25 ad 41 7f 3b 00 
```

```c
// Applying the inverse Key Schedule algorithm, it is possible to derive the master key from the fourth round key
KeyScheduleInverse(aes->key, 4);

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

## API

### Constants

* `BLOCK_SIZE = 16`
* `SET_SIZE = 256`

### AES functions

* `AES* create_aes_instance(unsigned char key[]);`:

* `void delete_aes_instance(AES* aes);`:

* `unsigned char * SubBytes (unsigned char data[]);`:

* `unsigned char * SubBytesInverse (unsigned char data[]);`:

* `unsigned char * ShiftRows (unsigned char data[]);`:

* `unsigned char * MixColumns (unsigned char data[]);`:

* `unsigned char * MixColumnsInverse (unsigned char data[]);`:

* `unsigned char * AddRoundKey (unsigned char data[], unsigned char roundkey[]);`:

* `unsigned char * AES4Rounds(AES* aes, unsigned char data[]);`:

### Key Schedule functions

* `unsigned char * KeySchedule (unsigned char key_in[], int round);`:

* `unsigned char * KeyScheduleInverse(unsigned char key_in[], int round);`:

### Square attack functions

* `unsigned char ReverseLastRound (unsigned char data_byte, unsigned char last_key_byte_guess);`:

* `int CheckGuess (unsigned char lambda_set[SET_SIZE][BLOCK_SIZE], int byte_position, unsigned char key_byte_guess);`:

* `unsigned char* SquareAttack(AES* aes);`:
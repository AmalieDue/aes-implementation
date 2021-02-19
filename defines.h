//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#ifndef AES_DEFINES_H
#define AES_DEFINES_H

/* 
* Multiply by 2
*/
#define multiply(x) (((x) << 1) ^ ((((x) >> 7) & 1) * 0x1b))

// General definitions
#define BLOCK_SIZE 16
#define SET_SIZE 256

#endif 
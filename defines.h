//
// Created by bla bla on 15/02/2021.
//

#ifndef AES_DEFINES_H
#define AES_DEFINES_H

/* Multiply
* Description:
*
*/
#define multiply(x) (((x) << 1) ^ ((((x) >> 7) & 1) * 0x1b))

// General definitions
#define BLOCK_SIZE 16
#define SET_SIZE 256

#endif 
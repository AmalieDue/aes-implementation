#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
* Addition in GF(2^8)
*/
uint8_t field_add(uint8_t a, uint8_t b) {
    return a^b;
}

/*
* p = a * b
*/
uint8_t field_mult(uint8_t a, uint8_t b) {
    uint8_t p = 0; /* the product of the multiplication */

    while (a && b) {
        if (b & 1) {
            p ^= a;
        }

        if (a & 0x80) {
            a = (a << 1) ^ 0x11b;
        } else {
            a <<= 1;
        }

        b >>= 1;
    }
    return p;
}

/*
* f(b) = 0x03 * b
*/
uint8_t xplus1_mult(uint8_t b) {
    uint8_t p = 0; /* the product of the multiplication */
    uint8_t xplus1 = '\x03';

    while (b && xplus1) {
        if (xplus1 & 1) {
            p ^= b;
        }

        if (b & 0x80) {
            b = (b << 1) ^ 0x11b;
        } else {
            b <<= 1;
        }

        xplus1 >>=1;
    }
    return p;
}

uint8_t M0[4] = {0x02, 0x03, 0x01, 0x01};
uint8_t M1[4] = {0x01, 0x02, 0x03, 0x01};
uint8_t M2[4] = {0x01, 0x01, 0x02, 0x03};
uint8_t M3[4] = {0x03, 0x01, 0x01, 0x02};

uint8_t* mix_columns(uint8_t* array) {
    uint8_t* out = malloc(sizeof(uint8_t)*sizeof(array));

    for (int i = 0; i < sizeof(array); i++) {
        out[0] ^= field_mult(array[i], M0[i]);
    }

    for (int i = 0; i < sizeof(array); i++) {
        out[1] ^= field_mult(array[i], M1[i]);
    }

    for (int i = 0; i < sizeof(array); i++) {
        out[2] ^= field_mult(array[i], M2[i]);
    }

    for (int i = 0; i < sizeof(array); i++) {
        out[3] ^= field_mult(array[i], M3[i]);
    }

    return out;
}

int main() {

    uint8_t poly = 244;
    uint8_t result = 0;

    result = xplus1_mult(poly);
    printf("Result of xplus1: %d\n", result);

    uint8_t polys[4] = {244, 243, 244, 244};
    uint8_t *polys_p = &polys[0];

    //uint8_t results[4];
    //uint8_t *results_p = &results[0];

    uint8_t *results;

    results = mix_columns(polys_p);
    printf("mix columns:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d\n", results[i]);
    }
    // uint8_t *p = &M0[0];

    // uint8_t arr[4];
    // uint8_t *arr_p = &arr[0];
    // arr_p = mix_columns(p);

    // for (int i = 0; i < sizeof(arr); i++) {
    //     printf("here %d\n", arr_p[i]);
    // }



//     uint8_t poly = 244;
//     uint8_t result;

//     result = xplus1_mult(poly);

//     printf("next result: %d", result);

//     uint8_t arr[4] = {0x00, 0x01, 0x02, 0x03};

//     for(int i = 0; i < sizeof(arr); i++) {
//         printf("i: %d\n", i);
//         printf("byte: %d\n", arr[i]);
//     } 

    free(results);
    return 0;
}


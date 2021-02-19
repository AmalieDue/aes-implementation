//
// Created by Amalie Due Jensen s160503 and Anders Lammert Hartmann s153596
//

#ifndef KEYSCHEDULE_AES_H
#define KEYSCHEDULE_AES_H

unsigned char * key_schedule (unsigned char key_in[], int round);

unsigned char * derive_master_key(unsigned char key_in[], int round);

#endif
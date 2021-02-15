//
// Created by bla bla
//

#ifndef KEYSCHEDULE_AES_H
#define KEYSCHEDULE_AES_H

unsigned char * KeySchedule (unsigned char key_in[], int round);

unsigned char * KeyScheduleInverse(unsigned char key_in[], int round);

#endif
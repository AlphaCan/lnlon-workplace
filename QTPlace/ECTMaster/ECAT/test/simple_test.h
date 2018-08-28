#ifndef __SIMPLE_TEST_H
#define __SIMPLE_TEST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include "ethercat.h"
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <pcap/pcap.h>

int mainFUN(void);
void servo_on(void);
void servo_off(void);
uint16 get_status_words(int slave);
int32_t get_position(int slave);
int32_t get_velocity(int slave);
int16_t get_torque(int slave);

void set_default_position(int slave);
void set_position(int slave,int speed);



typedef struct GROUP
{
    union
    {
        uint16_t statuswords;
        uint8_t word[2];
    };

    union
    {
        int32_t positon;
        int8_t pos[4];
    };

    union
    {
        int32_t velocity;
        int8_t vel[4];
    };

    union
    {
        int16_t torque;
        int8_t tor[2];
    };

}ACTUALDATA;



#ifdef __cplusplus
}
#endif

#endif

#ifndef __BEEP_H__
#define __BEEP_H__
#include "delay.h"
__sbit __at (0xA5) beep;
void Beep(unsigned int time){
    while(time>0){
        beep=0;
        Delay(2);
        beep=1;
        Delay(2);
        time--;
    }
}
#endif
#ifndef INPUT_H
#define INPUT_H

#include "delay.h"
#include "8052.h"
/*
 * 数字对应位置
 * 7  8  9 12
 * 4  5  6 13
 * 1  2  3 14
 * 10 0  11 15
*/

unsigned char scanner()
{
	unsigned char KeyNumber=17;
	
	P1=0xFF;
	P1_3=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=7;}//1
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=4;}//5
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=1;}//9
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=10;}//13取消
	
	P1=0xFF;
	P1_2=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=8;}//2
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=5;}//6
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=2;}//10
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=0;}//14
	
	P1=0xFF;
	P1_1=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=9;}//3
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=6;}//7
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=3;}//11
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=11;}//15确认
	
	P1=0xFF;
	P1_0=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=12;}//4
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=13;}//8
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=14;}//12
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=15;}//16
	
	return KeyNumber;
}



#endif
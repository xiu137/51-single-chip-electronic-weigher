#include <8052.h>
#include "LCD1602.h"
#include "input.h"
#include "delay.h"
#include "XPT2046.h"
#include "AT24C02.h"
#include "beep.h"
/*
 * 数字对应功能
 * 7    8    9     g/kg
 * 4    5    6     显示
 * 1    2    3     设置单价
 * 取消 0   确定   .
*/

/*
* 显示内容： 
    显示模式：           输入模式：
    weight：abc         new price：abc
    price： abc
*/
void mode0(){
    LCD_Init();
    LCD_ShowString(1,1,"weight:");
    LCD_ShowString(1,15,"kg");
    LCD_ShowChar(1,10,'.');
    LCD_ShowString(2,1,"Price:");
    LCD_ShowChar(2,10,'.');
    LCD_ShowString(2,15,"$");
}

unsigned int POW(unsigned char i){
    unsigned int result=1;
    while(i>0){
        result=result*10;
        i--;
    }
    return result;
}

void main(){
    unsigned char ADValue;//接收值为0-255
    unsigned int weightH;//0-10kg
    unsigned int weightL;//0-999g
    //mode 0 读取电阻值并显示   mode 1 设置单价
    unsigned char mode=0;
    unsigned int price=AT24C02_ReadByte(0x00);//高位
    Delay(10);
    price=price*256+AT24C02_ReadByte(0x01);//低位
    unsigned int temp_price;
    unsigned char point=0;
    unsigned long total_price;
    unsigned int total_priceH;
    unsigned int total_priceL;
    unsigned char input=0;
    unsigned char kg_g=1;//0为g，1为kg
    mode0();
    while(1){
        input=scanner();
        if(input==13){//显示
            mode=0;
            mode0();
            }
        else if(input==14){//设置单价
            mode=1;
            point=0;
            temp_price=0;
            LCD_Init();
            LCD_ShowString(1,1,"new Price:");
            if(kg_g==0)
                LCD_ShowString(2,13,"$/g");
            else
                LCD_ShowString(2,13,"$/kg");
            LCD_ShowChar(2,1,'>');
            }
        switch(mode){
            case 0:{//读取电阻值并显示
                ADValue=XPT2046_ReadAD(XPT2046_XP);
                weightH=ADValue*2/51;
                weightL=ADValue*2%51*1000/51;
                total_price=ADValue*2;
                total_price=total_price*price/51;
                total_priceH=(unsigned int)(total_price/1000);
                total_priceL=(unsigned int)(total_price%1000);
                LCD_ShowNum(1,8,weightH,2);
                LCD_ShowNum(1,11,weightL,3);
                LCD_ShowNum(2,7,total_priceH,3);
                LCD_ShowNum(2,11,total_priceL,3);
                // LCD_ShowNum(2,7,price,5);
                while(ADValue==255)
                {
                    Beep(100);
                    ADValue=XPT2046_ReadAD(XPT2046_XP);
                }
            }
            case 1:{//设置单价
                if(input==11){//确认
                    if(point==0)
                        point=1;
                    if(kg_g==1)
                        price=temp_price*POW(4-point);
                    else
                        price=temp_price;
                    AT24C02_WriteByte(0x00,price/256);
                    Delay(10);
                    AT24C02_WriteByte(0x01,price%256);
                    mode0();
                    mode=0;
                }
                else if(input==10){//取消
                    mode0();
                    mode=0;
                }
                else if(input<10){//输入数字
                    temp_price=temp_price*10+input;
                    LCD_ShowNum(0,0,input,1);
                    if(point>0)
                        point++;
                }
                else if(input==12)//g/kg
                {
                    if(kg_g==0){
                        kg_g=1;
                        LCD_ShowString(2,13,"$/kg");
                        LCD_ShowChar(2,1,'>');}
                    else{
                        kg_g=0;
                        LCD_ShowString(2,13,"$/g ");
                        LCD_ShowChar(2,1,'>');}
                }
                else if(input==15&&kg_g==1){
                    point=1;
                    LCD_ShowChar(0,0,'.');
                }

            }
        }
    }
}
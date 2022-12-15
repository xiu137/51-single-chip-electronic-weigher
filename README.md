# 智能电子秤设计

### 状态显示

电子秤显示一共有两种状态，就是显示模式和价格设置模式

- 显示模式下，显示内容为
  + **weight: ** +  (整数部分2位)  +  **.**  +  (小数部分3位) +  **kg**
  + **Price:** + (整数部分3位)  +  **.**  +  (小数部分3位) +   **kg**
- 价格设置模式下，显示内容为
  + **new price:**
  + \>          **$/kg**或者**$/g**

main函数开始，单片机会从AT24C02读取存储的价格然后根据根据按键来判断显示模式，确定显示模式后进入显示状态

#### 显示状态

单片机会读取AD值并计算相应重量和价格，最后显示出来

  ```c
  ADValue=XPT2046_ReadAD(XPT2046_XP);//读取AD值
  weightH=ADValue*2/51;//计算重量
  weightL=ADValue*2%51*1000/51;
  total_price=ADValue*2;
  total_price=total_price*price/51;//计算价格
  total_priceH=(unsigned int)(total_price/1000);
  total_priceL=(unsigned int)(total_price%1000);
  LCD_ShowNum(1,8,weightH,2);
  LCD_ShowNum(1,11,weightL,3);
  LCD_ShowNum(2,7,total_priceH,3);
  LCD_ShowNum(2,11,total_priceL,3);
  ```

  显示完成后会判断重量是否接近量程最大值，如果是则启用蜂鸣器

  ```c
  while(ADValue==255)
  {
       Beep(100);
       ADValue=XPT2046_ReadAD(XPT2046_XP);
  } 
  ```

#### 价格设置模式

单片机继续根据按键输入值来进行不同操作

进行小数点判断时如果输入状态为kg，启用小数点计量，并point设置为1

```c
else if(input==15&&kg_g==1){
     point=1;
     LCD_ShowChar(0,0,'.');
}
```

然后判断输入数字,如果存在小数点时，小数点位数+1

```c
else if(input<10){//输入数字
    temp_price=temp_price*10+input;
    LCD_ShowNum(0,0,input,1);
    if(point>0)
        point++;
}
```

按确认时读取输入状态是kg还是g，然后根据不同的状态写入输入值到price方便计算

```c
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
```


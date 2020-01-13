/*=======================================================================
 *Subsystem:   裸机
 *File:        GPIO.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _GPIO_H_
#define _GPIO_H_  

  #include  "TypeDefinition.h"
  #include  "MC9S12XEP100.h"
  //#include  "derivative.h" 

  //设置寄存器中某一位为1
  #define BSET(bit,Register)     ((Register)|= (1<<(bit)))
  //设置寄存器中某一位为0  
  #define BCLR(bit,Register)     ((Register) &= ~(1<<(bit))) 
  //得到寄存器中某一位状态
  #define BGET(bit,Register)     (((Register) >> (bit)) & 1) 

  //XEP100端口名(对应该端口数据寄存器)与地址的对应宏定义
  #define PA      0x00000000  
  #define PB      0x00000001
  #define PE      0x00000008
  #define PK      0x00000032
  #define PT      0x0240
  #define PS      0x0248     
  #define PM      0x0250
  #define PP      0x0258
  #define PH      0x0260
  #define PJ      0x0268
  #define PAD0    0x0270
  #define PAD1    0x0271

  //端口的各个寄存器间偏移地址的对应关系  ??????????????????????????????
  #define PRT  0     //数据寄存器
  #define PTI  1     //输入寄存器
  #define DDR  2     //方向寄存器
  #define RDR  3     //低功耗驱动寄存器
  #define PER  4     //上拉下拉使能寄存器
  #define PPS  5     //上拉下拉极性选择寄存器
  #define PIE  6     //引脚中断允许寄存器
  #define PIF  7     //引脚中断标志寄存器
  #define WOM  6     //引脚线或寄存器
  #define PRR  7     //引脚功能选择寄存器     

  extern uint8 QUICKCHARGE;

  typedef enum
  {
    Init_Normal_GPIO = 0
  };

  uint8 GPIO_Init(uint16 port,uint8 pin,uint8 direction,uint8 state);  
  uint8 GPIO_Get(uint16 port,uint8 reg,uint8 pin);                  
  void  GPIO_Set(uint16 port,uint8 reg,uint8 pin,uint8 state);  
  uint8 GPPort_Get(uint16 port,uint8 reg);
  void  GPPort_Set(uint16 port,uint8 reg,uint8 setFlag,uint8 bValue);

#endif 
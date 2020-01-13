//--------------------------------------------------------------------------*
// 文件名 : PIT.c                                                           *
// 说  明 : 该头文件为PIT(定时器)模块初始化及功能函数实现文件               *
//          (1)PITInit:  定时器初始化                                       *
//          (2)SecAdd1:定时器更新                                           *
//--------------------------------------------------------------------------*
//头文件包含，及宏定义区 
//头文件包含
#include "Includes.h"                  
 //构件函数实现

//PITInit:定时器初始化函数----------------------------------------------*
//功  能:定时器初始化，中断一次时间为1/38秒                             *
//参  数:无                                                             *
//返  回:无                                                             *
//----------------------------------------------------------------------*
  
void init_PIT0(void)
{
 
     PITMTLD0=24;     //为0通道8位计数器赋值
     PITLD0=1279;      //为0通道16位计数器赋值   //(249+1)*(1279+1)/32M=10ms
     PITMUX_PMUX0=0;   //第0通道使用微计数器0
     PITCE_PCE0=1;     //第0通道计数器工作 
     PITCFLMT=0X80;    //使能PIT模块
     PITINTE_PINTE0=1; //0通道定时器定时中断被使能
}
  
void init_PIT1(void)
{
 
     PITMTLD1=249;     //为1通道8位计数器赋值
     PITLD1=63999;     //为1通道16位计数器赋值   //(249+1)*(63999+1)=16000000个总线周期=0.5秒
     PITMUX_PMUX1=1;   //第1通道使用微计数器1
     PITCE_PCE1=1;     //第1通道计数器工作 
     PITCFLMT=0X80;    //使能PIT模块
     PITINTE_PINTE1=0; //0通道定时器定时中断被使能
}    
  
void PITInit(uint8 channel,uint8 MUXSEL,uint8 MTLD,uint16 LD)
{
      if (channel >= 7)
      {
          channel = 7;
      }
      //禁止PIT模块
      PITCFLMT_PITE = 0;
      // 使能PIT通道channel
      PITCE |= 1<<channel;
      //第channel通道使用微计数器MUXSEL,并要求的值填入对应的微定时加载寄存器中
      
      if (MUXSEL == 0) 
      {
        PITMUX &= ~(1<<channel);
        PITMTLD0 = MTLD;
      }
      else 
      {
        PITMUX |= 1<<channel;
        PITMTLD1 = MTLD;
      }
      // 定时器一次中断时间 = (PITMTLD + 1) * (PITLD + 1) / fBUS
      //                    =(249+1)*(63999+1)/32MHz=0.5s
      switch (channel)  
      {
        case 0:                                               
          PITLD0=LD;
          break;
        case 1:
          PITLD1=LD;
          break;
        case 2:
          PITLD2=LD;
          break;
        case 3:
          PITLD3=LD;
          break;
        case 4:
          PITLD4=LD;
          break;
        case 5:
          PITLD5=LD;
          break;
        case 6:
          PITLD6=LD;
          break;
        case 7:
          PITLD7=LD;
          break;
        default:
          break;    
      }
      
      // 使能PIT模块 
      PITCFLMT_PITE = 1;
      // 清通道0溢出标志,载入新的计时时间
      PITTF|=1<<channel;
      // 禁止PIT通道0中断 
      PITINTE &= ~(1<<channel);
}
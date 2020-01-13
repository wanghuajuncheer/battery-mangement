/*=======================================================================
 *Subsystem:   裸机
 *File:        Balance.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:   
      Author:  
      Modification:  
      
========================================================================*/
#include  "Includes.h"               

uint8   Balance_CFGR[NUM_IC][6];

/*=======================================================================
 *函数名:      Balance_Config();
 *功能:        对LTC6804的配置
 *参数:        无       
 *返回：       无
 *说明：       IC=3;GPIO=0XFF; REFON=1; SWTEN=0;  ADCOPT=0;  VUV=3.2; VOV=4.2; DCTO=0;
========================================================================*/
void Balance_Config( uint8 *CFG,uint8 gpio, uint8 refon,uint8 swtrd, uint8 adcopt,
                 float  vuv,float   vov,uint16 ddc,uint8 dcto) 
{           
    uint8 char1,char2,char3;
    uint8 current_ic;                             /* 对LTC6804_1的初始化命令配置参数 */

    *CFG++=(gpio<<3)+(refon<<2)+(swtrd<<1)+adcopt;
    
    *CFG++=(uint8)(((uint16)vuv*10000/16-1)&0x00ff);
    
    char1=(uint8)((((uint16)vov*10000/16)&0x000f)<<4);
 
    char2=(uint8)((((uint16)vuv*10000/16-1)&0x0f00)>>8);
     
    *CFG++=char1+char2;
    
    *CFG++= (uint8)(((uint16)(vov*10000/16)&0x0ff0)>>4);
    
    *CFG++= ddc;
    
    char3 = (uint8)(ddc>>8);
    
    char3 = char3 &0x0f; 
   
    *CFG++=(dcto<<4)+ char3;    

}
/*=======================================================================
 *函数名:      Passive_Balance（void）
 *功能:        
 *参数:        无       
 *返回：       无
 *说明：       被动均衡函数
========================================================================*/
void Passive_Balance(void) 
{
    uint8   i;
    uint8   Balance_Time;
    uint16  Diff_Volt,Value;
    uint16  Balance_Battery[3]={0,0,0};
                                                           /* 每次只均衡一个电池； */
   // i = VoltInfo.CellVolt_Max >>4;                      /* 第几排电池； */
    //VoltInfo.CellVolt_Max &=0x0F;                       /* 第几个电池； */
   // Balance_Battery[i] = VoltInfo.CellVolt_Max<<1;
    
  
    Balance_Battery[0] = 0x0001;
    
    Balance_Time = 0x01;                                  /* 设置均衡时间为30s; */ 
    
    Balance_Config(&Balance_CFGR[0][0],DGPIO ,DREFON,DSWTRD,DADCOPT,UNDER_V,OVER_V,0x0001,1) ;   /* 配置值赋给结构体,更改最后两个变量值；*/                                           

    Balance_Config(&Balance_CFGR[1][0],DGPIO ,DREFON,DSWTRD,DADCOPT,UNDER_V,OVER_V,Balance_Battery[1],Balance_Time) ;   /* 配置值赋给结构体,更改最后两个变量值；*/  

    Balance_Config(&Balance_CFGR[2][0],DGPIO ,DREFON,DSWTRD,DADCOPT,UNDER_V,OVER_V,Balance_Battery[2],Balance_Time) ;   /* 配置值赋给结构体,更改最后两个变量值；*/  


    LTC6804_wrcfg(NUM_IC,Balance_CFGR);
                                                                                     /* 再次对所有芯片进行配置；*/      
    Task_Flag_Counter.Counter_Balance++;
    
     
}




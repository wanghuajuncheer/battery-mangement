/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Cssu_Data_Collect.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:    2017 - 11 -10
      Author:  ZWB
      Modification:修改了LTC6804_Init（）函数中的DDCC值，直接去掉这个值。
                   原来数据传输有错误，打开了均衡模块。
                        
 * 2. Date:    2017 - 11 -11              
      Author:  ZWB
      Modification:Volt_Data_Process()中子板电压赋值问题，Total_Volt值要先计算结束，
                   再强制转换为16位，否则会出现数据丢失。           
========================================================================*/

#include  "Includes.h"                           /* 包含所有h文件，可以调用所有函数与变量 */ 
                                       
uint8  CFG1[NUM_IC][6];                          // 配置寄存器数组 ,用全局较好
uint16 Openwire_flag[NUM_IC];                   // 导线开路PEC计算 
/*=======================================================================
 *函数名:      Config_Fun();
 *功能:        对LTC6804的配置
 *参数:        无       
 *返回：       无
 *说明：       IC=3;GPIO=0XFF; REFON=1; SWTEN=0;  ADCOPT=0;  VUV=3.2; VOV=4.2; DCTO=0;
========================================================================*/
void Config_Fun( uint8 total_ic,uint8 gpio, uint8 refon,uint8 swtrd, uint8 adcopt,
                 float  vuv,float   vov,uint16 ddc,uint8 dcto) 
{           
   uint8 char1,char2,char3;
   uint8 current_ic;                             /* 对LTC6804_1的初始化命令配置参数 */
   
   for( current_ic = 0; current_ic<total_ic; current_ic++)
   {
      
      CFG1[current_ic][0]=(gpio<<3)+(refon<<2)+(swtrd<<1)+adcopt;
      
      CFG1[current_ic][1]=(uint8)(((uint16)vuv*10000/16-1)&0x00ff);
      
      char1=(uint8)((((uint16)vov*10000/16)&0x000f)<<4);
   
      char2=(uint8)((((uint16)vuv*10000/16-1)&0x0f00)>>8);
       
      CFG1[current_ic][2]=char1+char2;
      
      CFG1[current_ic][3]= (uint8)(((uint16)(vov*10000/16)&0x0ff0)>>4);
      
      CFG1[current_ic][4]= ddc;
      
      char3 = (uint8)(ddc>>8);
      
      char3 = char3 &0x0f; 
     
      CFG1[current_ic][5]=(dcto<<4)+ char3;    
   }
}

/*=======================================================================
 *函数名:      LTC6804_Init(void)
 *功能:        对LTC6804的初始化
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void LTC6804_Init(void)
{     
     
    Config_Fun(NUM_IC,DGPIO ,DREFON,DSWTRD,DADCOPT,UNDER_V,OVER_V,DDCC,DDCTW) ;     /* 配置值赋给结构体 */ 
                                               
    LTC6804_wrcfg(NUM_IC,CFG1);                                                     /* 写所有芯片配置寄存器 */
    
    set_adc(MD_NORMAL,DCP_ENABLED,CELL_CH_ALL,CELL_CHST_ALL, pup_up, chg);          /* 转换模式配置 */
}  

/*=======================================================================
 *函数名:      circuit_detection(void)
 *功能:        导线开路的检查
 *参数:        无       
 *返回：       无
 *说明：       通过上下拉电阻，压差大于4000则导线开路
========================================================================*/
void circuit_detection()
{   

    uint16 Cell_vol_down[NUM_IC][12];
    uint16 Cell_vol_up[NUM_IC][12];
    int16  Cell_vol_dif[NUM_IC][12];
    //uint16 Openwire_flag[NUM_IC];                   // 导线开路PEC计算
    uint16 Open_wire[NUM_IC][13];             // 导线开路状态
    
    uint8 *PEC_error_a;
    uint8  i,j;
    uint8  temp=0;
 
    set_adc(MD_NORMAL,1,CELL_CH_ALL,CELL_CHST_ALL,pup_up, chg);     // 转换PUP=1模式配置

    for(j = 0; j <250;j++)
    {                                                                         
        LTC6804_adow();                                                        // 设置ADOW命令
    }
    wakeup_idle();
    
    LTC6804_rdcv(0, NUM_IC, Cell_vol_up, PEC_error_a);                          // 读取AD电压值应该用电压采集函数
                                                                
    set_adc(MD_NORMAL,1,CELL_CH_ALL,CELL_CHST_ALL,pup_down, chg);   // 转换PUP=0模式配置

    for(j = 0;j <250;j++)
    {
        LTC6804_adow();                                                        // 设置ADOW命令
    }
    
    LTC6804_rdcv(0,NUM_IC, Cell_vol_down, PEC_error_a);                         // 读取AD电压值
    
    //Openwire_error=0x01;
    for(j = 0;j < NUM_IC;j++)
    {
        Openwire_flag[j]=0;
        for(i = 0;i < 13;i++) 
        {  
          Open_wire[j][i] = 0;
        }
    } 
    //13导线种类
    
    for(j = 0;j <NUM_IC;j++)                
    {
        switch(j)
         {
          case 0:
            for(i = 1;i < 12 ;i++)
            { 
              Cell_vol_dif[j][i] = Cell_vol_down[j][i] - Cell_vol_up[j][i];
              if(Cell_vol_dif[j][i] > 4000)
              Open_wire[j][i] = 1;                          // C1-11开路
            }
           if(Cell_vol_up[j][0] == 0)    
             Open_wire[j][0] = 1;                                   // C0开路
           if(Cell_vol_down[j][NUM1_Batper -1] == 0)                 
             Open_wire[j][NUM1_Batper ] = 1;                           // C12开路
           break;
         
          case 1:
            for(i = 1;i < 12 ;i++)
            { 
              Cell_vol_dif[j][i] = Cell_vol_down[j][i] - Cell_vol_up[j][i];
              if(Cell_vol_dif[j][i] > 4000)
              Open_wire[j][i] = 1;                          // C1-11开路
            }
           if(Cell_vol_up[j][0] == 0)    
             Open_wire[j][0] = 1;                                   // C0开路
           if(Cell_vol_down[j][NUM2_Batper -1] == 0)                 
             Open_wire[j][NUM2_Batper ] = 1;                           // C12开路
           break;
           
          case 2:
            for(i = 1;i < 12 ;i++)
            { 
              Cell_vol_dif[j][i] = Cell_vol_down[j][i] - Cell_vol_up[j][i];
              if(Cell_vol_dif[j][i] > 4000)
              Open_wire[j][i] = 1;                          // C1-11开路
            }
           if(Cell_vol_up[j][0] == 0)    
             Open_wire[j][0] = 1;                                   // C0开路
           if(Cell_vol_down[j][NUM3_Batper -1] == 0)                 
             Open_wire[j][NUM3_Batper ] = 1;                           // C12开路
           break;
           
          case 3:
            for(i = 1;i < 12 ;i++)
            { 
              Cell_vol_dif[j][i] = Cell_vol_down[j][i] - Cell_vol_up[j][i];
              if(Cell_vol_dif[j][i] > 4000)
              Open_wire[j][i] = 1;                          // C1-11开路
            }
           if(Cell_vol_up[j][0] == 0)    
             Open_wire[j][0] = 1;                                   // C0开路
           if(Cell_vol_down[j][NUM4_Batper -1] == 0)                 
             Open_wire[j][NUM4_Batper ] = 1;                           // C12开路
           break;
            
         case 4:
            for(i = 1;i < 12 ;i++)
            { 
              Cell_vol_dif[j][i] = Cell_vol_down[j][i] - Cell_vol_up[j][i];
              if(Cell_vol_dif[j][i] > 4000)
              Open_wire[j][i] = 1;                          // C1-11开路
            }
           if(Cell_vol_up[j][0] == 0)    
             Open_wire[j][0] = 1;                                   // C0开路
           if(Cell_vol_down[j][NUM5_Batper -1] == 0)                 
             Open_wire[j][NUM5_Batper ] = 1;                           // C12开路
           break;
            
           default:
             break;
        }
           
    }

    for(j = 0;j < NUM_IC;j++)
    {
      for(i = 0;i <13;i++) 
      {
        Openwire_flag[j]=Openwire_flag[j]+(Open_wire[j][i]<<i);
      }
    } 
    for(j = 0;j < NUM_IC;j++)
    {
     if(Openwire_flag[j]!=0)
      {
        VoltInfo.Openwire_error=1;
      }
    }
    Task_Flag_Counter.Counter_Wire_Open++;  
}
 

/*=======================================================================*/
/*=======================================================================
 *Subsystem:   裸机
 *File:        LTC6811_Init.C
 *Author:      ZWB
 *Description: 对6811进行初始化
 ========================================================================
 * History:    
 * 1. Date:    
      Author:  
      Modification:                     
           
========================================================================*/

#include  "includes.h" 
                         
uint8  CFG1[NUM_IC][6];                   // 配置寄存器数组 
/*=======================================================================
 *函数名:      Config_Fun();
 *功能:        对LTC6804的配置
 *参数:        无       
 *返回：       无
 *说明：       IC=3;GPIO=0XFF; REFON=1; SWTEN=0;  ADCOPT=0;  VUV=3.2; VOV=4.2; DCTO=0;
========================================================================*/
void Config_Fun( uint8 total_ic,uint8 gpio, uint8 refon,uint8 swtrd, uint8 adcopt,
                 float vuv, float vov, uint16 ddc, uint8 dcto) 
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
uint8 LTC6804_Init(void)
{     
  uint8 ltcstate;
  ltcstate = Spi_LTC6811Init();
  
  Config_Fun(NUM_IC,DGPIO ,DREFON,DSWTRD, DADCOPT,UNDER_V, OVER_V,DDCC, DDCTW);   /* 配置值赋给结构体 */ 
                                             
  LTC6804_wrcfg(NUM_IC, CFG1);                                                    /* 写所有芯片配置寄存器 */
  
  set_adc(MD_NORMAL, DCP_ENABLED,CELL_CH_ALL,CELL_CHST_ALL, pup_up, chg);         /* 转换模式配置 */
  
  return ltcstate;
}  

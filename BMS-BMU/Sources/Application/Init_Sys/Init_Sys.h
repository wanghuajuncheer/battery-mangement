/*=======================================================================
 *Subsystem:   裸机
 *File:        Init_Sys.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _INIT_SYS_H_
#define _INIT_SYS_H_  

  #include  "TypeDefinition.h"

  typedef struct//底层初始化标记位 00:成功;1:失败
  {
    uint8 ADC;
    uint8 IIC;
    uint8 PIT0;
    uint8 PLL;
    uint8 Relay_Positvie; //主正继电器
    uint8 EEPROM;
    //uint8 CAN0;
    uint8 CAN1;
    uint8 CAN2;
    //uint8 CAN3;
    uint8 Screen;
    uint8 SPI;
    uint8 Insul;//绝缘检测 
    uint8 AllInit_State;   
  }SysInitState_T;
  extern SysInitState_T g_SysInitState;
  
  void Init_Sys(void);
  
#endif  
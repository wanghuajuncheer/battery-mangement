/*=======================================================================
 *Subsystem:   裸机
 *File:        PLL.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _PLL_H_
#define _PLL_H_  

  #include  "TypeDefinition.h"
  #include  "MC9S12XEP100.h"
  #include  "derivative.h" 

  #define  BUS_CLOCK		   32000000	   //总线频率

  typedef enum
  {
    Init_Normal_PLL = 0,
    Init_Fault_PLL_PLLCLK
  };

  uint8 PLL_Init(void); 

#endif 
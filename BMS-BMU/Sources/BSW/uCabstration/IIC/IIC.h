/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        IIC.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _IIC_H_
#define _IIC_H_  

  #include  "TypeDefinition.h" 
  #include  "derivative.h"



  typedef enum
  {
    Init_Normal_IIC = 0
  };
  
  typedef enum
  {
    Normal_IICWrite = 0,
    Fault_IICWrite_IBIF1,
    Fault_IICWrite_RXAK1,
    Fault_IICWrite_IBIF2,
    Fault_IICWrite_RXAK2,
    Fault_IICWrite_IBIF3,
    Fault_IICWrite_RXAK3
  };
  
  uint8 IIC_Init(void);
  void IIC_write(uint8 addr,uint8 writeaddr,uint8 data);
  uint8 IIC_read(uint8 addr,uint8 readaddr); 

#endif                                                                           

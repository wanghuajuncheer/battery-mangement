/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        Init_IIC.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _INIT_IIC_H_
#define _INIT_IIC_H_  

  #include  "TypeDefinition.h"
  #include  "MC9S12XEP100.h"
  
  #define IIC_SCL       PTJ_PTJ7
  #define IIC_SDA       PTJ_PTJ6
  #define IIC_SCL_dir   DDRJ_DDRJ7
  #define IIC_SDA_dir   DDRJ_DDRJ6

  uint8 Init_IIC(void);

#endif
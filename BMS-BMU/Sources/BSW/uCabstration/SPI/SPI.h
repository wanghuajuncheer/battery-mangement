/*=======================================================================
 *Subsystem:   裸机
 *File:        SPI.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _SPI_H_
#define _SPI_H_

  #include  "MC9S12XEP100.h"
  #include  "TypeDefinition.h"

  //SPI1物理层初始化
  uint8  SPI1_Init(void);
  void   SPI1_Write(uint8 data); 
  uint8  SPI1_read(void);
  
#endif
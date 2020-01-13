/*=======================================================================
 *Subsystem:   裸机
 *File:        TypeDefinition.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#ifndef _TYPEDEFINITION_H_
#define _TYPEDEFINITION_H_

  typedef unsigned char         uint8;             //  8 位无符号数
  typedef unsigned short int    uint16;            // 16 位无符号数
  typedef unsigned long int     uint32;            // 32 位无符号数
  typedef char                  int8;              //  8 位有符号数
  typedef short int             int16;             // 16 位有符号数 
  typedef long  int             int32;             // 32 位有符号数
  	 
  //不优化变量类型,关键字volatile
  typedef volatile uint8        vuint8;            //  8 位无符号数 
  typedef volatile uint16       vuint16;           // 16 位无符号数 
  typedef volatile uint32       vuint32;           // 32 位无符号数 
  typedef volatile int8         vint8;             //  8 位有符号数 
  typedef volatile int16        vint16;            // 16 位有符号数 
  typedef volatile int32        vint32;            // 32 位有符号数 

  #define  abs(x) ((x)>0 ? (x):(-1*(x)))           //求绝对值的宏定义

#endif
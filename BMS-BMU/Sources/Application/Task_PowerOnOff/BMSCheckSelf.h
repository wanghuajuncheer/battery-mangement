 /*=======================================================================
 *Subsystem:   Âã»ú
 *File:        BMSCheckSelf.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _BMS_CHECK_SELF_H_
#define _BMS_CHECK_SELF_H_

  #include  "TypeDefinition.h"

  typedef struct
  {
    uint8   SysVolt_Over;
    uint8   SysVolt_Low;
    uint8   CellVolt_Low;
    uint8   CellVolt_Over;
    uint8   CellTemp_Low;
    uint8   CellTemp_Over;
    uint8   SysCurr_Over;
    uint8   SysInsul_Flt;
    uint8   OpenWire_Flt;     
  }Flt_BMSCheckSelf_T;
  extern  Flt_BMSCheckSelf_T  g_Flt_BMSCheckSelf;

 void BMS_WorkModeCheckself(void);
 
#endif
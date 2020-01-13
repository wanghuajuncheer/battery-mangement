#ifndef _LTC6811_VOLTCOLLECT_H_
#define _LTC6811_VOLTCOLLECT_H_
  
  #include  "TypeDefinition.h"
  #include  "LTC6811_ConnectType.h"
  
  typedef struct
  {
     uint16 CellVolt[NUM_Battery];
     uint16 CellVolt_Max;
     uint16 CellVolt_Min;
     uint8  CellVolt_MaxNode;
     uint8  CellVolt_MinNode;
     uint32 CellVolt_Total;
  }LTC6811_VoltInfo_T;
  extern LTC6811_VoltInfo_T g_LTC6811_VoltInfo;
  
  void LTC6811_VoltCMDSend(void);
  void LTC6811_VoltCollect(void);
  
#endif
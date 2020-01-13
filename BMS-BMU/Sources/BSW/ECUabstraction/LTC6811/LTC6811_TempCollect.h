#ifndef _LTC6811_TEMPCOLLECT_H_
#define _LTC6811_TEMPCOLLECT_H_

  #include  "TypeDefinition.h"
  #include  "LTC6811_CMDConfig.h"
  #include  "LTC6811_ConnectType.h"  

  typedef struct
  {
    uint8  CellTemp[NUM_Tem];
    uint8  CellTemp_Max;        //偏移量为40
    uint8  CellTemp_Min;        //偏移量为40
    uint8  CellTemp_MaxNode;    //最高温度节点
    uint8  CellTemp_MinNode;    //最低温度节点
    uint8  CellTemp_Ave;        //偏移量为40
    uint8  ICTemp[NUM_IC];      //偏移量为40
    uint8  ICTemp_OverState;    //芯片温度是否超标
    uint32 CellTemp_Tatoltemp;  //偏移量为40*NUM_Tem
  }LTC6811_TempInfo_T;
  extern LTC6811_TempInfo_T g_LTC6811_TempInfo;

  /* 温度处理函数； */

  void LTC6811_TempCMDSend(void);

  void LTC6811_TempCollect(void);

  void LTC6811_ChipTempCMDSend(void);

  void LTC6811_ChipTempCollect(void);



#endif
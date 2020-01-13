#ifndef _LTC6811_OPENWIREDETECT_
#define _LTC6811_OPENWIREDETECT_
 
  #include  "TypeDefinition.h"
  #include  "LTC6811_CMD.h"
  #include  "LTC6811_CMDConfig.h"
  #include  "LTC6811_ConnectType.h"                     //包含Num_IC的配置
  //输出的全局变量
  typedef struct
  {
    uint16 OpenwireLocation[NUM_IC];//导线开路的具体位置
    uint8  OpenwireErr;             //导线开路的总状态
  }LTC6811_OpwireInfo_T;
  extern LTC6811_OpwireInfo_T g_LTC6811_OpwireInfo;

  void LTC6811_OpenwireDetect(void);
  
#endif
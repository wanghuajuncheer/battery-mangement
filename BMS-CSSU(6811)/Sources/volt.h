#ifndef TASK_VOLT_PROCESS_
#define TASK_VOLT_PROCESS_

  #define NUM_pack               1//箱号


  #define NUM_Tem                5
  #define Cell_GPIO              2
  
  
  
  #define NUM_IC                 3
  #define NUM1_Batper            9
  #define NUM2_Batper            8  
  #define NUM3_Batper            8
  #define NUM4_Batper            0
  #define NUM5_Batper            0

  #define NUM1_Batper_true       8
  #define NUM1_Batper_front      4
  #define NUM1_Batper_rear       4
  #define NUM1_Batper_empty      2

  #define NUM2_Batper_true       8
  #define NUM2_Batper_front      4
  #define NUM2_Batper_rear       4
  #define NUM2_Batper_empty      2

  #define NUM3_Batper_true       9
  #define NUM3_Batper_front      5
  #define NUM3_Batper_rear       4
  #define NUM3_Batper_empty      1

  #define NUM4_Batper_true       0
  #define NUM4_Batper_front      0
  #define NUM4_Batper_rear       0
  #define NUM4_Batper_empty      0

  #define NUM5_Batper_true       0
  #define NUM5_Batper_front      0
  #define NUM5_Batper_rear       0
  #define NUM5_Batper_empty      0

  #define NUM_Battery      (NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)
   //#define NUM_Battery      NUM_IC*NUM_Batper


typedef struct
{
   uint16 CellVolt[25];
   uint16 CellVolt_Max;
   uint16 CellVolt_Min;
   uint8 CellVolt_MaxNode;
   uint8 CellVolt_MinNode;
   uint32 CellVolt_Total;
   uint8 Openwire_error;
}VoltInfo_T;


extern VoltInfo_T VoltInfo;


/* 电压采集与处理函数 */
void Task_Volt_Collect(void);

void Task_Volt_Process(void);



#endif
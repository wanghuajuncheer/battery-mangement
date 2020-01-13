#ifndef TASK_TEMP_PROCESS_
#define TASK_TEMP_PROCESS_

#define  SY_TEMP  0
#define  Temp_IC_over 110
typedef struct
{
  uint8 CellTemp[5];
  uint8 CellTemp_Max;
  uint8 CellTemp_Min;
  uint8 CellTemp_MaxNode;
  uint8 CellTemp_MinNode;
  //int8 CellTemp_Ave;
  int8 CellTemp_ic[NUM_IC];
  int16 CellTemp_tatoltemp;
  uint8 CellTemp_over_ic;
}TempInfo_T;
extern TempInfo_T TempInfo;



/* 温度处理函数； */

void Task_Pack_Temp_Collect(void);

void Task_Pack_Temp_Process(void);

void Task_Chip_Temp_Collect(void);

void Task_Chip_Temp_Process(void);


#endif
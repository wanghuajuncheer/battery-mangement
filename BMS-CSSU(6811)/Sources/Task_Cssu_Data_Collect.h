#ifndef TASK_CSSU_DATA_PROCESS_H
#define TASK_CSSU_DATA_PROCESS_H


/*************************************************************/
/*                          宏定义                           */
/*************************************************************/ 

//配置寄存器组的宏定义
#define  DGPIO      0xff                            //GPIO引脚控制
#define  DREFON     1                               //基准上电
#define  DSWTRD     0                               //SWTEN引脚状态
#define  DADCOPT    0                               //ADC模式选项位
#define  UNDER_V    2.50                            //设置欠压门阈值
#define  OVER_V     3.60                            //设置过压门阈值
#define  DDCC       0                               //关闭放电开关
#define  DDCTW      0                               //放电超时值（停用）
#define  DDCTO      2                               //放电超时值（ 1分）
#define  DCP_ENABLED    1                          //放电得到允许
#define  CELL_CH_ALL     0                          //Channels to convert :all cell
#define  CELL_CHST_ALL   0                          //状态组选择
#define  chg             0
#define  pup_up          1                          //上拉电流
#define  pup_down        0                          //下拉电流

//采集模式
#define MD_FAST 1          // ADCOPT = 0  27kHz; ADCOPT = 1  14kHz
#define MD_NORMAL 2        // ADCOPT = 0   7kHz; ADCOPT = 1   3kHz
#define MD_FILTERED 3      // ADCOPT = 0   26Hz; ADCOPT = 1   2kHz

//#define DCP_DISABLED 1

// 读取或者发送命令字节数与时间相对应
#define Time_4byte   50               // 微妙级别的，具体请看LTC680芯片的53页；
#define Time_28byte  350        
/**********************************************************/
/*                      *函数声明*                        */
/**********************************************************/
//导线开路检测
void circuit_detection();

//温度采集
//void messagedata_temp();

// 配置寄存器数组
void Config_Fun( uint8,uint8, uint8,uint8, uint8,
                 float,float,uint16,uint8);

/**************************************************************************/
/*                            变量extern                                  */
/**************************************************************************/ 


extern uint8  CFG1[NUM_IC][6];                          /* 配置寄存器数组 */
extern uint16 Openwire_flag[NUM_IC];                   // 导线开路PEC计算
#endif
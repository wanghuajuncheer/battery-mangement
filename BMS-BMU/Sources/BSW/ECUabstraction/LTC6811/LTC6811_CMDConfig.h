/*=======================================================================
 *Subsystem:   裸机
 *File:        LTC6811_CMDConfig.h
 *Author:      WenM
 *Description: 此配置文件主要是针对6811内部寄存器的相关配置
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#ifndef _LTC6811_CMD_CONFIG_H_
#define _LTC6811_CMD_CONFIG_H_
  
  //配置寄存器组的宏定义
  #define  DGPIO               0xff        // GPIO引脚控制
  #define  DREFON                 1        // 基准上电
  #define  DSWTRD                 0        // SWTEN引脚状态
  #define  DADCOPT                0        // ADC模式选项位
  #define  UNDER_V             2.45        // 设置欠压门阈值
  #define  OVER_V              3.75        // 设置过压门阈值
  #define  DDCC                   0        // 关闭放电开关
  #define  DDCTW                  0        // 放电超时值（停用）
  #define  DDCTO                  2        // 放电超时值（1分钟）
  #define  DCP_ENABLED            1        // 放电得到允许
  #define  CELL_CH_ALL            0        // Channels to convert :all cell
  #define  CELL_CHST_ALL          0        // 状态组选择
  #define  chg                    0
  #define  pup_up                 1        // 上拉电流
  #define  pup_down               0        // 下拉电流
  #define  Temp_IC_over         110        // 6811最高温度设定
  //采集模式
  #define MD_FAST                 1        // ADCOPT = 0  27kHz; ADCOPT = 1  14kHz
  #define MD_NORMAL               2        // ADCOPT = 0   7kHz; ADCOPT = 1   3kHz
  #define MD_FILTERED             3        // ADCOPT = 0   26Hz; ADCOPT = 1   2kHz

  //#define DCP_DISABLED 1

  //读取或者发送命令字节数与时间相对应
  //#define Time_4byte             50        // 微妙级别的，具体请看LTC680芯片的53页；
  //#define Time_28byte           350       

#endif
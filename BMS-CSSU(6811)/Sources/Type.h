//--------------------------------------------------------------------------*
// 文件名: Type.h (变量类型别名文件)                                        *
// 说  明: 定义变量类型的别名,目的:                                         *
//         (1)缩短变量类型书写长度;                                         *
//         (2)方便程序移植,可以根据需要自行添加.                            *                       
//--------------------------------------------------------------------------*

#ifndef TYPE_H                                   // 条件编译，防止重复定义  
#define TYPE_H
 
typedef unsigned char         uint8;             //  8 位无符号数
typedef unsigned short int    uint16;            // 16 位无符号数
typedef unsigned long int     uint32;            // 32 位无符号数
typedef char                  int8;              //  8 位有符号数
typedef short int             int16;             // 16 位有符号数 
typedef int                   int32;             // 32 位有符号数
	 
//不优化变量类型,关键字volatile
typedef volatile uint8        vuint8;            //  8 位无符号数 
typedef volatile uint16       vuint16;           // 16 位无符号数 
typedef volatile uint32       vuint32;      



// 32 位无符号数 
typedef volatile int8         vint8;             //  8 位有符号数 
typedef volatile int16        vint16;            // 16 位有符号数 
typedef volatile int32        vint32;            // 32 位有符号数 
	
//CAN报文结构体
typedef struct CanFrame
{
    uint32 m_ID;      // msg发送方ID
    uint8 m_IDE;      // 扩展帧为1，标准帧为0
    uint8 m_RTR;      // 远程帧为1，数据帧为0
    uint8 m_data[8];  // 帧数据
    uint8 m_dataLen;  // 帧数据长度
    uint8 m_priority; // 发送优先级 
}CANFRAME,* pCANFRAME;   

typedef struct Task_Time_Counter 
{
    uint16 Time_Flag600ms;
    uint16 Time_FlagGPS;
    uint16 Time_Handle;
    uint16 Time_Roll;
    //uint16 Time_IIC;
    uint16 Counter_Wire_Open;
    uint16 Counter_Volt_Collect;
    uint16 Counter_Volt_Process;
    uint16 Counter_Pack_Collect;
    uint16 Counter_Pack_Process;
    uint16 Counter_Chip_Collect;
    uint16 Counter_Chip_Process;
    uint16 BMS_to_UpMonitor;
    uint16 Counter_Control_Task;
    uint16 Counter_Balance_open;
    uint16 Counter_Balance_close; 
    uint16 Counter_Roll_Boot;
  
}Task_Time;
extern Task_Time Task_Flag_Counter;




#endif
#ifndef _CAN_H
#define _CAN_H

/*=======================================================================
 *函数名:      Boot的ID说明
 *功能:        说明boot升级过程中发送和返回的ID号,和数据
 *参数:        无
 *返回：       无 
 *说明：       1.主板和子板从应用程序中升级的ID号统一为:0xF300(扩展帧)
                 主板:Data[0]=0xF0,Data[1] =0xAA
                 子板:Data[0]=0,1,2按箱号类推,Data[1] =0xAA
               2.主板自Boot下载后直接升级的ID为:0x001(标准帧),boot程序返回数据
                 的ID号为:0x1FF,Data[0]=C3表示进入boot中可以升级,之后返回的数据
                 Data[0]=0,代表升级一行S19文件成功,否则那一行S19文件升级失败。
               3.子板自Boot下载后直接升级的ID为:0x002(标准帧)以此下推,boot程序返回数据
                 的ID号为:0x1FF,Data[0]=C3表示进入boot中可以升级,之后返回的数据
                 Data[0]=0,代表升级一行S19文件成功,否则那一行S19文件升级失败。 
========================================================================*/
//#define ID_BMU 	    0x0001//主板ID固定不变 
#define ID_Reply    0x01FF//主板和子板一致
#define ID01_6811   0x0002//子板的ID号以此类推
//以下为应用程序中升级的ID
//#define  Boot_ID        0xF300   //扩展帧

 
typedef struct                    //发送报文的结构体
{
    unsigned long id;             //ID号
    Bool RTR;                     //是否为远程贞
    unsigned char data[8];        //存放数据数组
    unsigned char len;            //can发送的数据长度为8  
    unsigned char prty;           //CAN通信传输优先级设定
} CAN_MSG;

extern CAN_MSG can_send;

#pragma CODE_SEG RAM_CODE
extern  interrupt void CAN2RxISR(void); 
#pragma CODE_SEG DEFAULT

extern INT8 getchar(void);
extern void INIT_CAN2(void); 
extern Bool MSCAN2GetMsg(CAN_MSG *msg);
extern Bool MSCAN2SendMsg(CAN_MSG msg);
#endif
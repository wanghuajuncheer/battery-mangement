#ifndef  _TASK_BOOT_H_
#define  _TASK_BOOT_H_


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
  #define  Boot_ID        0xF300       //扩展帧
  #define  CSSU_IDNUM     NUM_pack     //子板升级的第一个字节编号
  //以下为Boot程序中的ID命名
  //#define ID_BMU 	    0x0001//主板ID固定不变 
  //#define ID_Reply    0x01FF//主板和子板一致
  //#define ID01_6811   0x0002//子板的ID号以此类推
  //#define ID02_6811   0x0003//子板的ID号以此类推

  typedef struct
  {
     uint16 *Boot;
     uint8  OnlineUpdateCheck;      
  }Boot_Data_T;
  extern  Boot_Data_T Boot_Data;
  
  void Task_BootLoader();  
  
#endif
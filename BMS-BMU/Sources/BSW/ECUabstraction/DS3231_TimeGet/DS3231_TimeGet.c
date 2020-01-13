/*=======================================================================
 *Subsystem:   裸机
 *File:        DS3231_TimeGet.c
 *Author:      Wenming
 *Description: 通信：
               接口：PJ1，PJ0口
               波特率：100KHz
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "includes.h"
Read_IIC_Time_T Read_IIC_Time;  
/*=======================================================================
 *函数名:      BCD2HEX(uint8)
 *功能:        二进制转十进制任务
 *参数:        无       
 *返回：       无
 *说明：       将BCD码转换成16进制数,BCD码是用4位2进制数表示1个10进制数,低四位是十进制数，
               高四位是十进制数。例子：0x00100100;高四位就是2，低四位是4，就是24；
========================================================================*/
static
uint8 BCD2HEX(uint8 val) 
{
  uint8 value;
  value=val&0x0f;
  val>>=4;
  val&=0x0f;
  val*=10;
  value+=val;
  return value;
}

/*=======================================================================
 *函数名:      HEX2BCD(uint8)
 *功能:        二进制转十进制任务
 *参数:        无       
 *返回：       无
 *说明：       与上面的机制相反；
========================================================================*/
uint8 HEX2BCD(uint8 val) 
{
    uint8 i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
} 

 /*=======================================================================
 *函数名:      TimeInit_DelayTime(void)
 *功能:        Bootloader中的延时函数
 *参数:        us:延时时间,单位:us       
 *返回：       无
 *说明：       Bootloader中的延时函数
========================================================================*/
static
void DS3231_DelayTimeus(uint16 us) 
{
	  uint16 delayval;
	  delayval = us * 9;
	  while(delayval--);
}
 
 
/*=======================================================================
 *函数名:      DS3231SN_INIT()
 *功能:        对IIC模块进行清零
 *参数:        year:年,month:月,day:日       
 *返回：       无
 *说明：       此处用到BCD码,高4位为十进制的十位,低4位为十进制的个位
========================================================================*/
void DS3231SN_INIT(uint8 year, uint8 month, uint8 week, uint8 day, uint8 hour, uint8 min) // 这个函数第一次下载时可以使用一次
{
   IIC_write(0xd0,0x00,0x00);    /*将秒数初始化为0*/
   DS3231_DelayTimeus(10);
   IIC_write(0xd0,0x01,min);     /*将分钟初始化为0*/
   DS3231_DelayTimeus(10);
   IIC_write(0xd0,0x02,hour);    /*将小时数初始化为0*/
   DS3231_DelayTimeus(10);
   IIC_write(0xd0,0x03,week);     /*将周数初始化为0*/
   DS3231_DelayTimeus(10);
   IIC_write(0xd0,0x04,day);    /*将天初始化为0*/
   DS3231_DelayTimeus(10);
   IIC_write(0xd0,0x05,month);   /*将月数初始化为0*/
   DS3231_DelayTimeus(10);
   IIC_write(0xd0,0x06,year);    /*将年数初始化为0*/
   DS3231_DelayTimeus(100);      //写时间和读时间时间需要有时间间隔
}

/*=======================================================================
 *函数名:      DS3231_Read_Second()
 *功能:        读天数
 *参数:        无       
 *返回：       无
 *说明：       读取秒数，最大是59；
========================================================================*/
/*static
uint8 DS3231_Read_Second(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x00);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}*/
/*=======================================================================
 *函数名:      DS3231_Read_Minute()
 *功能:        读天数
 *参数:        无       
 *返回：       无
 *说明：       读取秒数，最大值59；
========================================================================*/
/*static
uint8 DS3231_Read_Minute(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x01);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}*/

/*=======================================================================
 *函数名:      DS3231_Read_Hour()
 *功能:        读小时数
 *参数:        无       
 *返回：       无
 *说明：       读取小时数，最大值是23小时；
========================================================================*/
/*static
uint8 DS3231_Read_Hour(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x02);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}*/

/*=======================================================================
 *函数名:      DS3231_Read_Day()
 *功能:        读天数
 *参数:        无       
 *返回：       无
 *说明：       读取天数，最大值是31天
========================================================================*/
/*static
uint8 DS3231_Read_Day(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x04);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}*/

/*=======================================================================
 *函数名:      DS3231_Read_Month()
 *功能:        读月数
 *参数:        无       
 *返回：       无
 *说明：       读取的月数，最大值是12月；
========================================================================*/
/*static
uint8 DS3231_Read_Month(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x05);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}*/

/*=======================================================================
 *函数名:      DS3231_Read_Year()
 *功能:        读年数
 *参数:        无       
 *返回：       无
 *说明：       读取年数，最大值是99年；
========================================================================*/
/*static
uint8 DS3231_Read_Year(void) 
{
  uint8 receivedata;
  receivedata = IIC_read(0xd0,0x06);
  receivedata = BCD2HEX(receivedata);
  return receivedata;
}*/

/*=======================================================================
 *函数名:      DS3231_Read_Year()
 *功能:        读总时间
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
 void DS3231_Read_Time(void) 
 {   
    uint8 receivedata;   
     
    receivedata=IIC_read(0xd0,0x01);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Minute = receivedata;
    DS3231_DelayTimeus(10);
    
    receivedata=IIC_read(0xd0,0x02);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Hour = receivedata;
    DS3231_DelayTimeus(10);
    
    receivedata=IIC_read(0xd0,0x04);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Day = receivedata;
    DS3231_DelayTimeus(10);
    
    receivedata=IIC_read(0xd0,0x05);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Month = receivedata;
    DS3231_DelayTimeus(10);
    
    receivedata=IIC_read(0xd0,0x06);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Year = receivedata;
 }

 
 

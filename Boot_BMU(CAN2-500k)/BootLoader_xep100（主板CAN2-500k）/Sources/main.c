#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Types.h"
#include "Config.h"
#include "CAN.h"
#include "Errors.h"
#include "PFlash.h"
#include "Srec.h"
#include "PartID.h"
#include "eeprom.h"

#pragma MESSAGE DISABLE C5909
#pragma MESSAGE DISABLE C12056

/******************************************************************************/
//Protect the bootloader at 0xF000-0xFFFF:

// const unsigned char flash_array[] @0xFF08 = {0xFF,0xFF,0xFF,0xFF,0xCF,0xFF,0xFF,0xFE};
/*
0xFF08-0xFF0B - reserved
0xFF0C - P-Flash Protection byte = 0xCF (Bootloader at 0xF000-0xFFFF is protected)
0xFF0D - EEE Protection byte = 0xFF (default state, protection is disabled)
0xFF0E - Flash Nonvolatile byte = 0xFF (default state, watchdog is disabled)
0xFF0F - Flash Security byte = 0xFE (default unsecured state)

Preload command file MUST contain command FLASH NOUNSECURE!
*/

/******************************************************************************/
#define RESET_VEC_SRC       0x7FFFE0        //0x7FFFE0
#define RESET_VEC_DST       0x7FEFF8        //0x7FEFF8

#define CPU12IVBR           0xFF00             //基地址
#define CAN1Ch              0xA2               //CAN0中断向量地址 = 基地址+0xD6

#define __SEG_START_REF(a)  __SEG_START_ ## a
#define __SEG_END_REF(a)    __SEG_END_ ## a
#define __SEG_SIZE_REF(a)   __SEG_SIZE_ ## a
#define __SEG_START_DEF(a)  extern INT8 __SEG_START_REF (a) []
#define __SEG_END_DEF(a)    extern INT8 __SEG_END_REF   (a) []
#define __SEG_SIZE_DEF(a)   extern INT8 __SEG_SIZE_REF  (a) []

__SEG_START_DEF (RAM_CODE);
__SEG_END_DEF   (RAM_CODE);
__SEG_SIZE_DEF  (RAM_CODE);

volatile  ErrType FlashErr;
unsigned int *boot;

/******************************************************************************/
static UINT8 EraseFlash(void)
{
  UINT8 Error;
  
  switch (PARTID)
  {
    
    //S12XE 1024k flash
    case MASK_0M48H:  //Falling to next case
    case MASK_1M48H:  //Falling to next case
    case MASK_2M48H:  //Falling to next case
    case MASK_3M48H:  //Falling to next case
    case MASK_4M48H_5M48H:
    case MASK_0N35H_1N35H:
          //erase whole P-Flash block 0 except bootloader area
          if(Error = PFlash_EraseSectorBySector(0x7C0000UL, 0x7FEFDFUL))
            return(Error); 
          //and erase all remaining blocks
          if(Error = PFlash_EraseBlock(0x7A0000UL))
            return(Error); 
          if(Error = PFlash_EraseBlock(0x780000UL))
            return(Error); 
          if(Error = PFlash_EraseBlock(0x740000UL))
            return(Error); 
          if(Error = PFlash_EraseBlock(0x700000UL))
            return(Error);
          return(noErr);
    
    default:
          can_send.data[0]=8;
          MSCAN2SendMsg(can_send);
          *boot=0x66;
          return(UnknownPartID);
  }      
}

/******************************************************************************/
static UINT8 ProgramFlash(void)
{
  SRecDataRec ProgSRec;
  UINT8 Error = noErr,i; 
  
  for(;;)
  {
    //go get an S-Record, return if there was an error
    if (Error = RcvSRecord(&ProgSRec)) 
    {
      
      break;
    }
    __asm nop;
    SrcdAlign4W(&ProgSRec);
    __asm nop;
    if (ProgSRec.RecType == EndRec)         // S7, S* or S9 record?
    {
      for(i=0;i<250;i++)
      {
        for(i=0;i<250;i++); 
      }
      COPCTL = 0x01;               //enable watchdog      
      ARMCOP = 0x00;
    
      break;                                // yes. return
    }
      
    else if (ProgSRec.RecType == HeaderRec) //S0 record?
      continue;             //yes. just ignore it
      
    else                    //a data record was received
    {

      if (ProgSRec.LoadAddr == RESET_VEC_SRC)          //复位向量的地址
      {
        //Program reset vector to address 0xEFFE
        if(Error = PFlash_Program(RESET_VEC_DST, (UINT16 *)&ProgSRec.Data[24]))
          return(Error);
      }   
      else
      {
        //is the address within a physical flash?
        switch (PARTID)
        {       
          //S12XE 1024k flash
          case MASK_0M48H:  //Falling to next case
          case MASK_1M48H:  //Falling to next case
          case MASK_2M48H:  //Falling to next case
          case MASK_3M48H:  //Falling to next case
          case MASK_4M48H_5M48H:
          case MASK_0N35H_1N35H:
              if(Error = PFlash_Program(ProgSRec.LoadAddr, (UINT16 *)&ProgSRec.Data))
                return(Error);
          
              if(Error = PFlash_Program(ProgSRec.LoadAddr+8, (UINT16 *)&ProgSRec.Data[8]))
                return(Error);
        
              if(Error = PFlash_Program(ProgSRec.LoadAddr+16, (UINT16 *)&ProgSRec.Data[16]))
                return(Error);
        
              if(Error = PFlash_Program(ProgSRec.LoadAddr+24, (UINT16 *)&ProgSRec.Data[24])) 
                return(Error);
                break;
                
          default:
                can_send.data[0]=8;
                MSCAN2SendMsg(can_send);
                *boot=0x66;
                return(UnknownPartID);
        } 
        
      }
      can_send.data[0]=0;
      MSCAN2SendMsg(can_send);
    }
  }
  
  return(Error);
}

/******************************************************************************/
static void CopyCodeToRAM(void)           //将RAM_CODE中的代码映射到RAM（0x3D00）中
{
  UINT8 *Src;
  UINT8 *Dst;
  UINT16  SegSize;
  UINT16  x;
  
  //RAM code resides in Flash from 0xfd00 - 0xfeff
  Src = (UINT8 *)__SEG_START_REF(RAM_CODE);
  //code will be copied to RAM at address 0x3D00
  Dst = (UINT8 *)0x3D00;
  
  SegSize = (UINT16)__SEG_SIZE_REF(RAM_CODE);
  
  for (x = 0; x < SegSize; x++)   //just copy a byte at a time
    *Dst++ = *Src++;
}

/******************************************************************************/
static void InterruptModuleSetup(void)
{
  //set the CPU12X interrupt vector base address in RAM 设置中断向量基地址在RAM中
  IVBR = (CPU12IVBR >> 8);
//  *(UINT16 *)(CPU12IVBR + CAN1Ch) = (UINT16)RxCAN;
}

/******************************************************************************/
static void PLL_Init(UINT8 synr, UINT8 refdv, UINT8 postdiv)
{
  PLLCTL = 0B10000001;  //CME=1,PLLON=0,FM1=0,FM2=0,FSTWKP=0,PRE=0,PCE=0,SCME=1
  CLKSEL = 0B00000011;  //PLLSEL=0,PSTP=0,PLLWAI=0,RTIWAI=1,COPWAI=1
  SYNR = synr;          //Set the multiplier register
  REFDV = refdv;        //Set the divider register
  POSTDIV = postdiv;    //Set the post divider register
  PLLCTL |= PLLCTL_PLLON_MASK;  //Enable the Phase Lock Loop开锁相环 0x40
  
  //Wait till the PLL VCO is within tolerance
  while((CRGFLG & CRGFLG_LOCK_MASK) == 0);    //0x08
  
  CLKSEL |= CLKSEL_PLLSEL_MASK; //system clocks are derived from来自 PLLCLK 0x80
}

/******************************************************************************/
void main(void) 
{
  UINT8 i;
  DisableInterrupts;
  boot = (unsigned int*)0x0C00;
  PLL_Init(SYNR_VALUE, REFDV_VALUE, POSTDIV_VALUE); //set 40MHz BUSCLK
  FCLKDIV = FLASH_PRESCALER;  //set flash prescaler 0x0F
  INIT_CAN2();            //initialize CAN
  CopyCodeToRAM();
  InterruptModuleSetup();
  
  Init_Eeprom(&FlashErr);
  FlashErr = LaunchFlashCommand(2 ,PARTITION_D_FLASH, 0, DFPART, EEE_RAM, 0, 0, 0, 0, 0);
  ErrorCheck(FlashErr,(accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));
  if(FlashErr.ErrCode==NoErr) 
  {
     FlashErr = LaunchFlashCommand(0 ,EEPROM_QUERY, 0, 0, 0, 0, 0, 0, 0, 0); 
     ErrorCheck(FlashErr,(accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));
     ChecPartErr(&FlashErr);   
  }  
  if(FlashErr.ErrCode == NoErr)  //无错误使能EEPROM
  {
     FlashErr = LaunchFlashCommand(0 ,ENABLE_EEPROM_EMULATION, 0, 0, 0, 0, 0, 0, 0, 0);
     ErrorCheck(FlashErr, (accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));    
  }
  *boot = 0;
  while(*boot != 0)
  {
    *boot = 0;
  }
  EraseFlash();
  can_send.data[0]=0xC3;
  MSCAN2SendMsg(can_send);
  EnableInterrupts;       //enable interrupts for the CAN

  for(;;)
  {
    ProgramFlash();
  }
  
}
/******************************************************************************/


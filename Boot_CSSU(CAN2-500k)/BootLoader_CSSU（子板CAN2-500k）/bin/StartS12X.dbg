;********************************************************************************
;* Freescale Semiconductor Inc.
;* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
;* ALL RIGHTS RESERVED.
;********************************************************************************
;Services performed by FREESCALE in this matter are performed AS IS and without 
;any warranty. CUSTOMER retains the final decision relative to the total design 
;and functionality of the end product. FREESCALE neither guarantees nor will be 
;held liable by CUSTOMER for the success of this project.
;FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
;BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
;A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE OR ADVISE SUPPLIED TO THE PROJECT
;BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
;SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
;THIS AGREEMENT.
;
;CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
;actions by anyone on account of any damage, or injury, whether commercial, 
;contractual, or tortuous, rising directly or indirectly as a result of an advise
;or assistance supplied CUSTOMER in connection with product, services or goods 
;supplied under this Agreement.
;********************************************************************************
;* File      StartS12X.s
;* Owner     b01802
;* Version   1.0   
;* Date      Dec-02-2010
;* Classification   General Business Information
;* Brief     Startup file
;********************************************************************************
;* Detailed Description:
;*
;*    Tested on: DEMO9S12XSFAME, EVB9S12XEP100
;*    
;*    The bootloder handles all reset vectors. This startup file makes a decision
;*    whether the bootloader or user application is executed. 
;********************************************************************************
;Revision History:
;Version   Date          Author    Description of Changes
;1.0       Dec-02-2010  b01802    Initial version
;********************************************************************************
;
  xref  main            ;xref表示本子程序的定义在其他文件中
;
  xdef  _BootStart      ;xdef表示在本文件中定义了可以被其他文件中的程序调用的子程序
;

AppResetVect:   equ     $effe ;equ，把该地址赋给AppResetVect标号 here is stored reset vector of user application
StackTop:       equ     $3900 ;栈顶地址
BootCtrWord:    equ     $0C00 ;EEPROM的0C00位置用于储存BootLoader控制字


_BootStart:

  ; -----------------------------------------------------------------------------
  ldd   BootCtrWord
  cpd   #$66
  beq   GoBoot          ;如果bootloader控制字是0x66说明要进入bootloader模式，进行升级
                        ;否则进入下一步判断AppResetVect的值，有效进入应用程序，无效进入Bootloader
  
 
  ; -----------------------------------------------------------------------------
  
  ldd   AppResetVect    ;将这个数加载到累加器D中
  cpd   #$ffff          ;将累加器D的内容与这个数比较
  beq   GoBoot          ; 如果地址无法使用(超程)就进入BOOT程序中if the application reset vector is not available
                        ; then start the bootloader
  ldx   AppResetVect     ;将App地址加载到累加器X中
  jmp    0,x              ; jump to the application
  
  
GoBoot:
  lds  #StackTop
  jmp  main
  
;********************************************************************************


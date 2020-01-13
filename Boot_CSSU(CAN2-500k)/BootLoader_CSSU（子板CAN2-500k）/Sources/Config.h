/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without 
any warranty. CUSTOMER retains the final decision relative to the total design 
and functionality of the end product. FREESCALE neither guarantees nor will be 
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      Config.h
* Owner     b01802
* Version   1.0 
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Configuration of bootloader
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Dec-02-2010   b01802    Initial version
*******************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H


/******************************************************************************/
// --- 1 ---
/******************************************************************************/
//set the flash prescaler according to reference manual to achieve operating
//frequency 800kHz - 1050kHz.

#define FLASH_PRESCALER 0x0F  //flash prescaler for 16MHz oscillator
//#define FLASH_PRESCALER 0x07  //flash prescaler for 8MHz oscillator
//#define FLASH_PRESCALER 0x03  //flash prescaler for 4MHz oscillator


/******************************************************************************/
// --- 2 ---
/******************************************************************************/
//PLL settings to achieve 40MHz bus clock
//Use PLL calculator (attached to this bootloader) if you have oscillator which
//is not defined below. 

//16MHz oscillator:
#define SYNR_VALUE      0x44
#define REFDV_VALUE     0x81
#define POSTDIV_VALUE   0x00

//8MHz oscillator:
//#define SYNR_VALUE      0x44
//#define REFDV_VALUE     0x80
//#define POSTDIV_VALUE   0x00

//4MHz oscillator:
//#define SYNR_VALUE      0x49
//#define REFDV_VALUE     0x40
//#define POSTDIV_VALUE   0x00


#endif
/******************************************************************************/

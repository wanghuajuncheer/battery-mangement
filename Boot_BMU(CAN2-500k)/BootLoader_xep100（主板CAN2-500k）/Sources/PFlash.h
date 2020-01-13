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
* File      PFlash.h
* Owner     b01802
* Version   1.0 
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Program and Erase functions
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Dec-02-2010   b01802    Initial version
*******************************************************************************/
#ifndef _FLASH_H 
#define _FLASH_H

#pragma CODE_SEG RAM_CODE

extern UINT8 PFlash_Program(UINT32 address, UINT16 *ptr);
extern UINT8 PFlash_EraseSector(UINT32 address);
extern UINT8 PFlash_EraseSectorBySector(UINT32 addr_l, UINT32 addr_h);
extern UINT8 PFlash_EraseBlock(UINT32 address);

#pragma CODE_SEG DEFAULT

#endif
/******************************************************************************/

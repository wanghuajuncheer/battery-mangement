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
* File      PartID.h
* Owner     b01802
* Version   1.0 
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     List of supported mask sets and part IDs
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Dec-02-2010   b01802    Initial version
*******************************************************************************/
#ifndef _PARTID_H
#define _PARTID_H


//S12XE:
#define MASK_0M48H 0xCC90       //1024k flash
#define MASK_1M48H 0xCC91
#define MASK_2M48H 0xCC92
#define MASK_3M48H 0xCC93
#define MASK_4M48H_5M48H 0xCC94
#define MASK_0N35H_1N35H 0xCC95

#define MASK_0M25J 0xC480       //512k flash
#define MASK_1M25J 0xC481
#define MASK_2M25J_3M25J 0xC482

#define MASK_0M53J 0xC080       //256k flash
#define MASK_1M53J_2M53J 0xC081



#endif

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
* File      DeviceInfo.h
* Owner     b01802
* Version   1.0 
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Definiton of used register
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Dec-02-2010   b01802    Initial version
*******************************************************************************/
#ifndef _REGDEF_H 
#define _REGDEF_H

#define IVBR        (*(volatile UINT8 *)  0x00000121)       //中断向量基址寄存器，用于设定中断向量地址高位的值
#define PLLCTL      (*(volatile UINT8 *)  0x0000003A)
#define CLKSEL      (*(volatile UINT8 *)  0x00000039)
#define CRGFLG      (*(volatile UINT8 *)  0x00000037)
#define SYNR        (*(volatile UINT8 *)  0x00000034)
#define REFDV       (*(volatile UINT8 *)  0x00000035)
#define POSTDIV     (*(volatile UINT8 *)  0x00000036)
#define COPCTL      (*(volatile UINT8 *)  0x0000003C)
#define ARMCOP      (*(volatile UINT8 *)  0x0000003F)
#define FCLKDIV     (*(volatile UINT8 *)  0x00000100)
#define SCIDRL      (*(volatile UINT8 *)  0x000000CF)
#define SCICR2      (*(volatile UINT8 *)  0x000000CB)
#define SCISR1      (*(volatile UINT8 *)  0x000000CC)
#define FSTAT       (*(volatile UINT8 *)  0x00000106)
#define FCCOBIX     (*(volatile UINT8 *)  0x00000102)
#define FCCOB       (*(volatile UINT16 *) 0x0000010A)
#define SCIBD       (*(volatile UINT16 *) 0x000000C8)
#define PARTID      (*(volatile UINT16 *) 0x0000001A)

#define SCICR2_SBK_MASK                 1
#define SCICR2_RWU_MASK                 2
#define SCICR2_RE_MASK                  4
#define SCICR2_TE_MASK                  8
#define SCICR2_ILIE_MASK                16
#define SCICR2_RIE_MASK                 32
#define SCICR2_TCIE_MASK                64
#define SCICR2_TIE_MASK                 128

#define SCISR1_PF_MASK                  1
#define SCISR1_FE_MASK                  2
#define SCISR1_NF_MASK                  4
#define SCISR1_OR_MASK                  8
#define SCISR1_IDLE_MASK                16
#define SCISR1_RDRF_MASK                32
#define SCISR1_TC_MASK                  64
#define SCISR1_TDRE_MASK                128

#define FSTAT_MGSTAT0_MASK              1
#define FSTAT_MGSTAT1_MASK              2
#define FSTAT_MGBUSY_MASK               8
#define FSTAT_FPVIOL_MASK               16
#define FSTAT_ACCERR_MASK               32
#define FSTAT_CCIF_MASK                 128

#define CPMUFLG_LOCK_MASK               8

#define PLLCTL_PLLON_MASK               64
#define CRGFLG_LOCK_MASK                8
#define CLKSEL_PLLSEL_MASK              128

#endif
/******************************************************************************/

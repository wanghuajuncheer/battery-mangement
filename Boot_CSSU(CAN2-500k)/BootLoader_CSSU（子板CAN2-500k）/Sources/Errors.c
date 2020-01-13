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
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE OR ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      Errors.c
* Owner     b01802
* Version   1.0   
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Error functions 
********************************************************************************
Revision History:
Version   Date          Author    Description of Changes
1.0       Dec-02-2010   b01802    Initial version
*******************************************************************************/
#include <string.h>
#include "derivative.h" /* derivative-specific definitions */
#include "Types.h"
#include "Errors.h"

/******************************************************************************/
static INT8 * const ErrorString[] = {
                                      NULL,
                                      "S-Record Out Of Range",
                                      "S-Record Size Must Be Even",
                                      "Flash Programming Error",
                                      "Flash Erase Error",
                                      "Bad Hex Data",
                                      "S-Record Too Long",
                                      "CheckSumErr",
                                      "Unknown Part ID"
                                    };

const char BadError[] = "Invalid Error Number";

/******************************************************************************/
INT8 * GetErrorString(UINT8 ErrorNumber)
{
  INT16 MaxError;
  
  
  MaxError = sizeof(ErrorString) / sizeof(INT8 *);
  
  if (ErrorNumber > MaxError)
    return((INT8 * const)BadError);
  else
    return(ErrorString[ErrorNumber]);
  
}
/******************************************************************************/

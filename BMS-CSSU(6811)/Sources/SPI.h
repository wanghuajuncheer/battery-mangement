#ifndef SPI_H_
#define SPI_H_

/* 子板专用的SPI1 */
#define  SS2          PTP_PTP3
#define  SS2_Dir      DDRP_DDRP3

#define  SD_CS_Select()     PTP_PTP6=0
#define  SD_CS_Release()    PTP_PTP6=1
#define  CS_Dir             DDRP_DDRP6



void SPI1_Init(void);
void SPI1_Write(uint8 data); 
  
uint8 SPI1_read(void);

#endif
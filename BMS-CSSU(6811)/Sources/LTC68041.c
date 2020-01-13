/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        LTC68041.C
 *Author:      ZWB
 *Description: Í¨ÐÅ£º
               ½Ó¿Ú£º
               ²¨ÌØÂÊ£º
 ========================================================================
 * History:    ÐÞ¸ÄÀúÊ·¼ÇÂ¼ÁÐ±í
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "Includes.h"                 /* °üº¬ËùÓÐhÎÄ¼þ£¬¿ÉÒÔµ÷ÓÃËùÓÐº¯ÊýÓë±äÁ¿ */ 

uint8 ADCV[2];                         // Cell Voltage conversion command.
uint8 ADSTAT[2];                       // state conversion command.
uint8 ADOW[2];                         // µ¼Ïß¿ªÂ·.
uint8 ADAX[2];                         // GPIO conversion command.

/*=======================================================================
 *º¯ÊýÃû:      wakeup_idle() 
 *¹¦ÄÜ:        SPI»½ÐÑº¯Êý
 *²ÎÊý:        ÎÞ       
 *·µ»Ø£º       ÎÞ
 *ËµÃ÷£º       
========================================================================*/
void delay_time(uint16 t)
{
  uint16 i;
  for(i=0; i<t; i++);
  
}


void wakeup_idle()
{
  SS2=0;
  delay_time(5);                             // Guarantees the isoSPI will be in ready mode
  SS2=1;
}
/*******************************************************/
/*  brief Wake the LTC6804 from the sleep state        */
/* *****************************************************/
 
void wakeup_sleep()
{
  SS2 = 0;
  delay_time(5);                             // Guarantees the LTC6804 will be in standby
  SS2 =1;
}
  
/*************************************************************/
/*                      Ð´ÅäÖÃ¼Ä´æÆ÷º¯Êý                     */
/*************************************************************/ 
void LTC6804_wrcfg(uint8 total_ic, uint8 config[][6])
{
  const uint8 BYTES_IN_REG = 6;
  const uint8 CMD_LEN = 4+(8*total_ic);
  uint8   *cmd;
  uint16  cfg_pec;
  uint8   cmd_index;                   // command counter
  uint8   current_ic;
  uint8   current_byte;
  uint8  i,j;
  
  
  
  
  cmd = (uint8 *)malloc(CMD_LEN*sizeof(uint8));
  
  //1
  cmd[0] = 0x00;
  cmd[1] = 0x01;
  cmd[2] = 0x3d;
  cmd[3] = 0x6e;
  
  //2
  cmd_index = 4;
  for ( current_ic = total_ic; current_ic > 0; current_ic--) 		                     	// executes for each LTC6804 in daisy chain, this loops starts with
  {																			                                            	// the last IC on the stack. The first configuration written is
      for ( current_byte = 0; current_byte < BYTES_IN_REG; current_byte++)            // executes for each of the 6 bytes in the CFGR register
      {																		                                           	// current_byte is the byte counter
	         cmd[cmd_index] = config[current_ic-1][current_byte]; 						          //adding the config data to the array to be sent 
           cmd_index = cmd_index + 1;                
      }
    	//3
      cfg_pec = (uint16)pec15_calc(BYTES_IN_REG, &config[current_ic-1][0]);		      // calculating the PEC for each ICs configuration register data
      cmd[cmd_index] = (uint8)(cfg_pec >> 8);
      cmd[cmd_index + 1] = (uint8)cfg_pec;
      cmd_index = cmd_index + 2;
  }
  
  //4
  wakeup_idle (); 															                                    	//This will guarantee that the LTC6804 isoSPI port is awake.This command can be removed.
  
  //5
  SS2=0;
  spi_write_array(CMD_LEN, cmd);
  delay_time(100);  //28¸ö¶ÑÕ»
  SS2=1;
  free(cmd);
} 

/*************************************************************/
/*                     ÉèÖÃADC×ª»»ÃüÁî                       */
/************************************************************/
void set_adc(uint8 MD,uint8 DCP,uint8 CH,uint8 CHST,uint8 PUP, uint8 CHG)
{
    uint8 md_bits;
    md_bits = (MD & 0x02) >> 1;
    ADCV[0] = md_bits + 0x02;        
    md_bits = (MD & 0x01) << 7;
    ADCV[1] =  md_bits + 0x60 + (DCP<<4) + CH;
    
    md_bits = (MD & 0x02) >> 1;
    ADOW[0]=md_bits+0x02;
    md_bits= (MD & 0x01) << 7;
    ADOW[1]=  md_bits+(DCP<<4) + CH+0x28+ (PUP<<6) ;
    
    md_bits = (MD & 0x02) >> 1;
    ADSTAT[0] = md_bits + 0x04;        
    md_bits = (MD & 0x01) << 7;
    ADSTAT[1] = md_bits + 0x68 + CHST;
    
    md_bits = (MD & 0x02) >> 1;
    ADAX[0] = md_bits + 0x04;
    md_bits = (MD & 0x01) << 7;
    ADAX[1] = md_bits + 0x60 + CHG;
}

/*************************************************************/
/*                      µ¼Ïß¿ªÂ·¼ì²é                         */
/*************************************************************/
void LTC6804_adow() 
{
  uint8  cmd[4];
  uint16 cmd_pec;
  
  cmd[0] = ADOW[0];
  cmd[1] = ADOW[1];
  
  cmd_pec = pec15_calc(2,ADOW);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec);
  
  wakeup_idle();
  SS2=0;   
  spi_write_array(4,cmd);
  delay_time(500);
  SS2=1;   
}


/*************************************************************/
/*          Æô¶¯µç³ØµçÑ¹ADCÂÖÑ¯×ª»»º¯Êý                      */
/*************************************************************/ 
void LTC6804_adcv()
{

  uint8 cmd[4];
  uint16 cmd_pec;
  
  //1
  cmd[0] = ADCV[0];
  cmd[1] = ADCV[1];
  
  //2
  cmd_pec = pec15_calc(2, ADCV);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec);
  
  //3
  wakeup_idle (); //This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.
  
  //4
  SS2=0;
  spi_write_array(4,cmd);
  delay_time(70);
  SS2=1;
  //delay_time(100);
}
/*************************************************************/
/*          Æô¶¯GPIO ADCÂÖÑ¯×ª»»º¯Êý                         */
/*************************************************************/ 
void LTC6804_adax()
{
  uint8  cmd[4];
  uint16 cmd_pec;
 
  cmd[0] = ADAX[0];          // 0x05;
  cmd[1] = ADAX[1];          // 0x60;
  
  cmd_pec = pec15_calc(2, ADAX);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec);
 
  wakeup_idle (); //This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.

  SS2 = 0;
  spi_write_array(4,cmd);
  delay_time(70);
  SS2 = 1;
  //delay_time(100);                     // DYZ+ 4.25

}
 
/*************************************************************/
/*          Æô¶¯×´Ì¬×éADCÂÖÑ¯×ª»»º¯Êý                        */
/*************************************************************/ 
void LTC6804_adstat()
{
    uint8 cmd[4];
    uint16 cmd_pec;

    cmd[0] = ADSTAT[0];           // 0x05;
    cmd[1] = ADSTAT[1];           // 0x68;
    
    cmd_pec = pec15_calc(2, ADSTAT);
    cmd[2] = (uint8)(cmd_pec >> 8);
    cmd[3] = (uint8)(cmd_pec);

    wakeup_idle();
    
    SS2=0;
    spi_write_array(4,cmd);
    delay_time(70);       
    SS2=1;
}

/*************************************************************/
/*          ¶ÁÈ¡µç³ØµçÑ¹¼Ä´æÆ÷×éABCD                         */
/*************************************************************/
/* reg:¼Ä´æÆ÷¸öÊý£¬total_ic=3,dataÎª´ÓµçÑ¹¼Ä´æÆ÷ÖÐ¶Áµ½µÄÊý¾Ý */
/*************************************************************/
void LTC6804_rdcv_reg(uint8 reg, uint8 total_ic, uint8 *data )
{
  const uint8 REG_LEN = 8;             // number of bytes in each ICs register + 2 bytes for the PEC
  uint8 cmd[4];
  uint16 cmd_pec;
  
  //1
  if (reg == 1)                        // 1: RDCVA
  {
    cmd[1] = 0x04;
    cmd[0] = 0x00;
  }
  else if(reg == 2)                    // 2: RDCVB
  {
    cmd[1] = 0x06;
    cmd[0] = 0x00;
  } 
  else if(reg == 3)                    // 3: RDCVC
  {
    cmd[1] = 0x08;
    cmd[0] = 0x00;
  } 
  else if(reg == 4)                    // 4: RDCVD
  {
    cmd[1] = 0x0A;
    cmd[0] = 0x00;
  } 

  //2
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec); 
  
  //3
  wakeup_idle ();                      // This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.
  
  //4
  SS2=0;
  spi_write_read(cmd,4,data,(REG_LEN*total_ic));
  delay_time(60);
  SS2=1;
}    
/*************************************************************/
/*                  ¶ÁÈ¡µç³ØµçÑ¹Öµº¯Êý                       */
/*************************************************************/
/*reg:Ñ¡Ôñ¶ÁÈ¡·½Ê½Îª4¸ö¼Ä´æÆ÷Ò»Æð¶Á£¬»¹ÊÇ·Ö¿ªµ¥¸ö¼Ä´æÆ÷¶ÁÈ¡  */
/*************************************************************/ 
void LTC6804_rdcv(uint8 reg,  uint8 total_ic, uint16 cell_codes[][12], uint8 *ppec)
{
  
  const uint8 NUM_RX_BYT = 8;                    // ½ÓÊÜ8¸ö×Ö½Ú£¬Èý¸öµçÑ¹Öµ£¬Ò»¸öPEC£»
  const uint8 BYT_IN_REG = 6;
  const uint8 CELL_IN_REG = 3;
  
  uint8  pec_error[4] ={0,0,0};
  uint16 received_pec[NUM_IC*4];
  uint16 data_pec[NUM_IC*4];
  
  uint8  data_counter=0;                         // data counter
  uint8  cell_reg;                               // ¼Ä´æÆ÷counte
  uint8  current_ic;                             // IC counter
  uint8  current_cell;                           // cell counter
  uint16 parsed_cell;                            // ¹ý¶ÈÁ¿
  
  uint8  *cell_data;                             // ½ÓÊÕdata
 
  cell_data = (uint8 *)malloc((NUM_RX_BYT*total_ic)*sizeof(uint8));  // ¿ª±ÙÁË24¸ö×Ö½ÚµÄ¿Õ¼ä£¬Ã¿´Î¸ÕºÃÓÃÍê8*3£»
  
  if (reg == 0)                                  // ¶ÁÈ¡·½Ê½Ñ¡Ôñ£¬È«²¿¶ÁÈ¡¡£
  {
     for(cell_reg = 1; cell_reg<5; cell_reg++)	 // Ö´ÐÐËÄ´Î£¬Ã¿´Î£¬¶ÁÒ»×é¼Ä´æÆ÷µÄÖµ£¬Ò»¹²ÓÐABCDËÄ×é£»
     {
        data_counter = 0; 
                                                      
        LTC6804_rdcv_reg(cell_reg,total_ic,cell_data );						   // ¶ÁÒ»×é¼Ä´æÆ÷µÄÖµ
	      
        for ( current_ic = 0 ; current_ic <total_ic; current_ic++)   // executes for every LTC6804 in the daisy chain
        {																 	  		                                                  
	        	for(current_cell = 0; current_cell<CELL_IN_REG; current_cell++)	                       // ÕâÑùµÄ±à³Ì£¬¿ÉÒÔÖ±½ÓµçÑ¹¶ÔºÅÈë×ù£¬µçÑ¹Êý¾Ý´¦Àí²»ÓÃ¸Ä±äÖµ¡£    	
            {														   		  		                                     
                parsed_cell = cell_data[data_counter] + (cell_data[data_counter + 1] << 8);        // ½ÓÊÜµÄÒ»¸öµçÑ¹Öµ½øÐÐ¸³Öµ£»         
                cell_codes[current_ic][current_cell  + ((cell_reg - 1) * CELL_IN_REG)] = parsed_cell;
                data_counter = data_counter + 2;														                                                	
            }
            
            received_pec[current_ic+(cell_reg-1)*total_ic] = (cell_data[data_counter] << 8) + cell_data[data_counter+1];  // PECÐ£ÑéµÄÖµ£»
		        data_pec[current_ic+(cell_reg-1)*total_ic] = pec15_calc(BYT_IN_REG, &cell_data[current_ic * NUM_RX_BYT]);
		    
            if(received_pec[current_ic+(cell_reg-1)*total_ic] != data_pec[current_ic+(cell_reg-1)*total_ic])              // ½ÓÊÜµÄPECÐ£ÑéÓëÍ¨¹ýÈý¸öµçÑ¹Êý¾Ý¼ÆËãµÄ½øÐÐ±È½Ï£»
            {
                // PECÐ£ÑéµÄÖµ£¬²»ÊÇ¶ÔºÅÈë×ùµÄ£¬¶øÊÇ°´ÕÕË³ÐòÀ´µÄ¡£
                *(ppec+current_ic+(cell_reg-1)*total_ic)=1;														                                  	// Èç¹û²»ÏàµÈ£¬ËµÃ÷´«ËÍ¹ý³Ì³öÏÖ´íÎó£»
               
            }
            //data_counter=data_counter+2;																												
        } 
     }
  }
 //1.b
  else
  {
	  //Ä³¸ö¼Ä´æÆ÷
    LTC6804_rdcv_reg(reg, total_ic,cell_data);  
    //IC
    for ( current_ic = 0 ; current_ic < total_ic; current_ic++) 			                           	  // executes for every LTC6804 in the daisy chain
    {																 	  		                                                      	// current_ic is used as the IC counter
		   //Èý¸öµç³Ø
		   for( current_cell = 0; current_cell < CELL_IN_REG; current_cell++)                            // This loop parses the read back data into cell voltages, it 
       {
		    	parsed_cell = cell_data[data_counter] + (cell_data[data_counter+1]<<8); 																	
		  	  cell_codes[current_ic][current_cell + ((reg - 1) * CELL_IN_REG)] = 0x0000FFFF & parsed_cell;
		    	data_counter= data_counter + 2;     								                                      	//Because cell voltage codes are two bytes the data counter
      	}
		   // Èý¸öICµÄPEC
	       received_pec[current_ic+(cell_reg-1)*total_ic] = (cell_data[data_counter] << 8 )+ cell_data[data_counter + 1];      //The received PEC for the current_ic is transmitted as the 7th and 8th
         data_pec[current_ic+(cell_reg-1)*total_ic] = pec15_calc(BYT_IN_REG, &cell_data[current_ic * NUM_RX_BYT]);
	       if(received_pec[current_ic+(cell_reg-1)*total_ic] != data_pec[current_ic+(cell_reg-1)*total_ic])
	     	 {													                                   	//The pec_error variable is simply set negative if any PEC errors 
		      	*(ppec+current_ic+(cell_reg-1)*total_ic)=1;																		              //are detected in the serial data
	       }
	  	   data_counter= data_counter + 2; 										                                      	//Because the transmitted PEC code is 2 bytes long the data_counter		  																                                                        //must be incremented by 2 bytes to point to the next ICs cell voltage data
  	}
 }
  free(cell_data);
}
/*************************************************************/
/*                 ¶ÁÈ¡¸¨Öú¼Ä´æÆ÷×éAB                        */
/*************************************************************/ 
/*reg:¸¨Öú¼Ä´æÆ÷AB                                           */
/*************************************************************/ 
void LTC6804_rdaux_reg(uint8 reg, uint8 total_ic,uint8 *data)
{
  const uint8 REG_LEN = 8;                     // number of bytes in the register + 2 bytes for the PEC
  
  uint8  cmd[4];
  uint16 cmd_pec;
  
  //1
  if (reg == 1)		  	                          // Read back auxiliary group A
  {
    cmd[1] = 0x0C;
    cmd[0] = 0x00;
  }
  else if(reg == 2)		                         // Read back auxiliary group B 
  {
    cmd[1] = 0x0E;
    cmd[0] = 0x00;
  } 
  //2
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec);
  
  //3
  wakeup_idle ();                                // This will guarantee that the LTC6804 isoSPI port is awake, this command can be removed.
  
  //4
  SS2 = 0;
  spi_write_read(cmd,4,data,(REG_LEN*total_ic));
  delay_time(60);
  SS2 = 1;
}
/*************************************************************/
/*                     ¶ÁÈ¡ÎÂ¶Èº¯Êý                          */
/*************************************************************/ 
/*reg:Ñ¡Ôñ¶ÁÈ¡·½Ê½Îª2¸ö¼Ä´æÆ÷Ò»Æð¶Á£¬»¹ÊÇ·Ö¿ªµ¥¸ö¼Ä´æÆ÷¶ÁÈ¡  */
/*************************************************************/ 
void LTC6804_rdaux(uint8 reg,  uint8 total_ic, uint16 aux_codes[][6],	uint8 *ppec2)
{
  const uint8 NUM_RX_BYT = 8;
  const uint8 BYT_IN_REG = 6;
  const uint8 GPIO_IN_REG = 3;
  
  uint8  pec_error[NUM_IC];
  uint16 received_pec[NUM_IC];
  uint16 data_pec[NUM_IC];
  
  uint8  data_counter = 0;                     // data counter
  uint8  gpio_reg;                             // ¼Ä´æÆ÷counter 
  uint8  current_ic;                           // IC counter
  uint8  current_gpio;                         // cell counter
  uint16 parsed_aux;
 
  uint8 *data;   
  data = (uint8 *) malloc((NUM_RX_BYT*total_ic)*sizeof(uint8));
   
  if (reg == 0)
  {
     for(gpio_reg = 1; gpio_reg < 3; gpio_reg++)		 	   		 			                             //executes once for each of the LTC6804 aux voltage registers
     {
        data_counter = 0;
        LTC6804_rdaux_reg(gpio_reg, total_ic,data);								                        	  //Reads the raw auxiliary register data into the data[] array
	  
        for (current_ic = 0 ; current_ic < total_ic; current_ic++) 		                        // executes for every LTC6804 in the daisy chain
        {																 	  			                                            // current_ic is used as the IC counter
	         for(current_gpio = 0; current_gpio< GPIO_IN_REG; current_gpio++)               	  // This loop parses the read back data into GPIO voltages, it 
           {														   		  			                                        // loops once for each of the 3 gpio voltage codes in the register 
              parsed_aux = data[data_counter] + (data[data_counter+1]<<8);                    //Each gpio codes is received as two bytes and is combined to
						  aux_codes[current_ic][current_gpio +((gpio_reg-1)*GPIO_IN_REG)] = parsed_aux;		// create the parsed gpio voltage code
					  	data_counter=data_counter+2;															
           }
           received_pec[current_ic] = (data[data_counter]<<8)+ data[data_counter+1]; 				  //The received PEC for the current_ic is transmitted as the 7th and 8th
		       data_pec[current_ic] = pec15_calc(BYT_IN_REG, &data[current_ic*NUM_RX_BYT]);
          if(received_pec[current_ic] != data_pec[current_ic])
          {
              pec_error[current_ic] = 1;														                        	//The pec_error variable is simply set negative if any PEC errors 
				    	*(ppec2+current_ic)=pec_error[current_ic];															       	//are detected in the received serial data
          } 
          data_counter=data_counter+2;											                                 	//Because the transmitted PEC code is 2 bytes long the data_counter
	     }
      }
 }
 
 else
 {
	//b.i
    LTC6804_rdaux_reg(reg, total_ic, data);
    for (current_ic = 0 ; current_ic < total_ic; current_ic++) 			  	                    	// executes for every LTC6804 in the daisy chain
    {							   									          		                                         // current_ic is used as an IC counter
		//b.ii
		  for(current_gpio = 0; current_gpio<GPIO_IN_REG; current_gpio++)  	 	                  // This loop parses the read back data. Loops 
		  {						 											  	                                                // once for each aux voltage in the register 
	    	parsed_aux = (data[data_counter] + (data[data_counter+1]<<8));    		              //Each gpio codes is received as two bytes and is combined to
				aux_codes[current_ic][current_gpio +((reg-1)*GPIO_IN_REG)] = parsed_aux;
		  	data_counter=data_counter+2;									 	                                  	//Because gpio voltage codes are two bytes the data counter
	  	}
		//b.iii
	     	received_pec[current_ic] = (data[data_counter]<<8) + data[data_counter+1]; 				  //The received PEC for the current_ic is transmitted as the 7th and 8th
		    data_pec[current_ic] = pec15_calc(BYT_IN_REG, &data[current_ic*NUM_RX_BYT]);
        if(received_pec[current_ic] != data_pec[current_ic])
        {
          pec_error[current_ic] = -1;													   	                        	//The pec_error variable is simply set negative if any PEC errors 
				 	*(ppec2+current_ic)=pec_error[current_ic];																      	//are detected in the received serial data
        }
      	data_counter=data_counter+2;											                                	//Because the transmitted PEC code is 2 bytes long the data_counter
  	}
}
  free(data);
}
   
/*************************************************************/
/*                     ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷×é                      */
/*************************************************************/
void LTC6804_rdstat_reg(uint8 reg, uint8 total_ic, uint8 *data )
{
    const uint8 REG_LEN = 8; // number of bytes in the register + 2 bytes for the PEC
    
    uint8  cmd[4];
    uint16 cmd_pec;

    //1
    if(reg == 1)			//Read back auxiliary group A
    {
    cmd[1] = 0x10;
    cmd[0] = 0x00;
    }
    else if(reg == 2)		//Read back auxiliary group B 
    {
    cmd[1] = 0x12;
    cmd[0] = 0x00;
    } 
       
    //2
    cmd_pec = pec15_calc(2, cmd);
    cmd[2] = (uint8)(cmd_pec >> 8);
    cmd[3] = (uint8)(cmd_pec);

    //3
    wakeup_idle (); //This will guarantee that the LTC6804 isoSPI port is awake, this command can be removed.

    //4
    SS2=0;
    spi_write_read(cmd,4,data,(REG_LEN*total_ic));
    delay_time(60);
    SS2=1;
} 
 
/*************************************************************/
/*          ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷×éº¯Êý                             */
/*************************************************************/ 
uint8 LTC6804_rdstat(uint8 reg, uint8 total_ic,uint16 state_codes[][6], uint8 *ppec )
{
  const uint8 NUM_RX_BYT = 8;
  const uint8 BYT_IN_REG = 6;
  const uint8 STATE_IN_REG = 3;
  
  uint8  pec_error[3]={ 0,0,0 };
  uint16 received_pec[3];
  uint16 data_pec[3];
 
  uint8  data_counter = 0; 
  uint8  state_reg;
  uint8  current_ic;
  uint8  current_state;
  uint16 parsed_aux;

  uint8 *data;
  data = (uint8 *) malloc((NUM_RX_BYT*total_ic)*sizeof(uint8));
  
  
  //1.a
  if (reg == 0)
  {
  	//a.i
    for(state_reg = 1; state_reg < 3; state_reg++)		 	   		 			                                  //executes once for each of the LTC6804 aux voltage registers
    {
       data_counter = 0;
       LTC6804_rdstat_reg(state_reg, total_ic, data);					                                      	//Reads the raw auxiliary register data into the data[] array
	     for ( current_ic = 0 ; current_ic < total_ic; current_ic++) 		                               	// executes for every LTC6804 in the daisy chain
       {	
       //a.ii
	        	for( current_state = 0; current_state< STATE_IN_REG; current_state++)                   	// This loop parses the read back data into GPIO voltages, it 
            {														   		  			                                                // loops once for each of the 3 gpio voltage codes in the register 
                parsed_aux = data[data_counter] + (data[data_counter+1]<<8);                          //Each gpio codes is received as two bytes and is combined to
		            state_codes[current_ic][current_state +((state_reg-1)*STATE_IN_REG)] = parsed_aux;
                data_counter=data_counter+2;											                                  	//Because gpio voltage codes are two bytes the data counter
            }
	        	//a.iii
           received_pec[current_ic] = (data[data_counter]<<8)+ data[data_counter+1]; 			         	 //The received PEC for the current_ic is transmitted as the 7th and 8th
		       data_pec[current_ic] = pec15_calc(BYT_IN_REG, &data[current_ic*NUM_RX_BYT]);
           if(received_pec[current_ic] != data_pec[current_ic])
           {
              pec_error[current_ic] = 1;
             *(ppec+current_ic)=pec_error[current_ic];															               //The pec_error variable is simply set negative if any PEC errors 
	         }
           data_counter=data_counter+2;											                                     	//Because the transmitted PEC code is 2 bytes long the data_counter
	    }
   }
 
 }
 else
 {
	  //b.i
    LTC6804_rdstat_reg(reg,total_ic, data);
    for ( current_ic = 0 ; current_ic < total_ic; current_ic++) 			  	                         	 // executes for every LTC6804 in the daisy chain
    {
       //b.ii
		   for( current_state = 0; current_state<STATE_IN_REG; current_state++)  	                      	// This loop parses the read back data. Loops 
	    	{						 											  	                                                       	// once for each aux voltage in the register 
		        parsed_aux = (data[data_counter] + (data[data_counter+1]<<8));    	                    	//Each gpio codes is received as two bytes and is combined to
		        state_codes[current_ic][current_state +((reg-1)*STATE_IN_REG)] = parsed_aux;
		      	data_counter=data_counter+2;									 	                                        	//Because gpio voltage codes are two bytes the data counter
      	}
	     	//b.iii
	    	received_pec[current_ic] = (data[data_counter]<<8) + data[data_counter+1]; 			          	 //The received PEC for the current_ic is transmitted as the 7th and 8th
	      data_pec[current_ic] = pec15_calc(BYT_IN_REG, &data[current_ic*NUM_RX_BYT]);
        if(received_pec[current_ic] != data_pec[current_ic])
        {
           pec_error[current_ic]= 1;													   		                                 //The pec_error variable is simply set negative if any PEC errors 
				   *(ppec+current_ic)=pec_error[current_ic];															                  	//are detected in the received serial data
        }
        
	   	data_counter=data_counter+2;												                                           //Because the transmitted PEC code is 2 bytes long the data_counter
	  }
 }
 free(data);
 return (*pec_error);
}        
/*************************************************************/
/*          Çå³ýµçÑ¹¼Ä´æÆ÷×éº¯Êý                             */
/*************************************************************/ 

void Ltc6804_Clrcell()
{
  uint8  cmd[4];
  uint16 cmd_pec;
  
  //1
  cmd[0] = 0x07;                       // Çå³ýµçÑ¹¼Ä´æÆ÷×éÃüÁî£»
  cmd[1] = 0x11;
  
  //2
  cmd_pec = pec15_calc(2, cmd);        // PECÐ£Ñé£»
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec );
  
  //3
  wakeup_idle ();                      // »½ÐÑSPIÍ¨ÐÅ£»
  //delay_time(100);
  //4
  SS2=0;                               // °ÑCSBÀ­µ½µÍµçÆ½£»
  spi_write_read(cmd,4,0,0);           // ·¢ËÍÃüÁîµ½SPI£»
  delay_time(70);                      // ¾ßÌåµÄÊ±¼ä¼ÆËãÇë¿´LTC6804Ð¾Æ¬ÊÖ²á53Ò³                   
  SS2=1;                               // °ÑCSBÀ­µ½¸ßµçÆ½£
  //delay_time(100);
}
/*************************************************************/
/*                    Çå³ý¸¨Öú¼Ä´æÆ÷×é                       */
/*************************************************************/ 

void Ltc6804_clraux()
{
  uint8  cmd[4];
  uint16 cmd_pec;
  
  //1
  cmd[0] = 0x07;                       // Çå³ýµçÑ¹¼Ä´æÆ÷×éÃüÁî£»
  cmd[1] = 0x12;
  
  //2
  cmd_pec = pec15_calc(2, cmd);        // PECÐ£Ñé£»
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec );
  
  //3
  wakeup_idle ();                      // »½ÐÑSPIÍ¨ÐÅ£»
  //delay_time(100);                     // DYZ+ 4.25
  //4
  SS2=0;                               // °ÑCSBÀ­µ½µÍµçÆ½£»
  spi_write_read(cmd,4,0,0);           // ·¢ËÍÃüÁîµ½SPI£»
  delay_time(70);              // ¾ßÌåµÄÊ±¼ä¼ÆËãÇë¿´LTC6804Ð¾Æ¬ÊÖ²á53Ò³                   
  SS2=1;                               // °ÑCSBÀ­µ½¸ßµçÆ½£»
  //delay_time(100);                     // DYZ+ 4.25

}
/*************************************************************/
/*                    Çå³ý×´Ì¬¼Ä´æÆ÷×é                       */
/*************************************************************/ 

void Ltc6804_Clrstat()
{
  uint8  cmd[4];
  uint16 cmd_pec;
  
  //1
  cmd[0] = 0x07;                       // Çå³ýµçÑ¹¼Ä´æÆ÷×éÃüÁî£»
  cmd[1] = 0x13;
  
  //2
  cmd_pec = pec15_calc(2, cmd);        // PECÐ£Ñé£»
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec );
  
  //3
  wakeup_idle ();                      // »½ÐÑSPIÍ¨ÐÅ£»
  
  //4
  SS2=0;                               // °ÑCSBÀ­µ½µÍµçÆ½£»
  spi_write_read(cmd,4,0,0);           // ·¢ËÍÃüÁîµ½SPI£»
  delay_time(70);              // ¾ßÌåµÄÊ±¼ä¼ÆËãÇë¿´LTC6804Ð¾Æ¬ÊÖ²á53Ò³                   
  SS2=1;                               // °ÑCSBÀ­µ½¸ßµçÆ½£»
}
/*************************************************************/
/*                         ¼ÆËãPECº¯Êý                       */
/*************************************************************/
uint16 pec15_calc(uint8 len,uint8 *data)
{
  uint8 i;
	uint16 remainder,addr;
	remainder = 16;                                      //initialize the PEC
	for( i = 0; i<len;i++)                               // loops for each byte in data array
	{
	  	addr = ((remainder>>7)^data[i])&0xff;           //calculate PEC table address 
		  remainder = (remainder<<8)^crc15Table[addr];
	}
	return(remainder*2);                               //The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}
/*************************************************************/
/*                        SPIÐ´Êý¾Ýº¯Êý                      */
/*************************************************************/
void spi_write_array(uint8 len,uint8 data[])
{
    uint8 i;
    for (i = 0; i < len; i++)
    {
        SPI1_Write((uint8)data[i]);
    }
}
/*************************************************************/
/*                        SPI¶ÁÊý¾Ýº¯Êý                      */
/*************************************************************/
void spi_read(uint8 *rx_data,uint8 rx_len)
{
    uint8 i;
    
    for (i = 0; i < rx_len; i++)
    {
        rx_data[i] = (uint8)SPI1_read();
    }
}  
/*************************************************************/
/*                        SPI¶ÁÐ´Êý¾Ýº¯Êý                      */
/*************************************************************/
void spi_write_read(uint8 tx_Data[],uint8 tx_len,uint8 *rx_data,uint8 rx_len)
{
    uint8 i;
    uint8 temp1;
    
    for (i = 0; i < tx_len; i++)       // ·¢ËÍÃüÁî¸øLTC6804Ð¾Æ¬£»
    {
        SPI1_Write(tx_Data[i]);
    }  
    
    if(rx_len !=0) 
    {
      
        SPI1_Write(0x00);
        delay_time(500);
        temp1 = (uint8)SPI1_read();
           
      
        for (i = 0; i < rx_len; i++)
        {
            SPI1_Write(0x00);
            rx_data[i] = (uint8)SPI1_read();
        }
    }
}
/*=======================================================================
 *º¯ÊýÃû:      int8 HXYA_Gpio_Search(uint16 adc) 
 *¹¦ÄÜ:        ÎÂ¶È²é±í
 *²ÎÊý:        ÎÞ       
 *·µ»Ø£º       ÎÞ
 *ËµÃ÷£º       HXYAÊ¹ÓÃµÄÎÂ¶È²é±íº¯Êý 
========================================================================*/
int8 HXYA_Gpio_Search(uint16 adc)     
{
/*
uint16 ltc6804gpio_ADC_table[161] =              // 10k  -40¡ã~120¡ã
{  
28583,28505,28423,28338,28249,28157,28060,27960,27856,27747,
27634,27517,27395,27268,27138,27002,26862,26716,26566,26412, 
26252,26087,25917,25742,25562,25377,25187,24992,24791,24586, 
24376,24162,23942,23718,23489,23255,23017,22775,22529,22279, 
22025,21767,21506,21241,20973,20702,20429,20153,19874,19594, 
19311,19027,18741,18454,18167,17878,17589,17299,17010,16721, 
16432,16143,15856,15569,15284,15000,14718,14437,14159,13882, 
13608,13337,13068,12802,12538,12278,12021,11767,11516,11269, 
11025,10785,10548,10316,10086, 9861, 9639, 9422, 9208, 8998,
 8792, 8589, 8391, 8197, 8006, 7819, 7636, 7457, 7282, 7110,
 6942, 6778, 6617, 6460, 6306, 6156, 6009, 5866, 5726, 5590,
 5456, 5326, 5199, 5074, 4953, 4835, 4720, 4607, 4498, 4391,
 4286, 4184, 4085, 3988, 3894, 3802, 3713, 3625, 3540, 3457,
 3376, 3297, 3220, 3145, 3072, 3001, 2932, 2864, 2799, 2734,
 2672, 2611, 2552, 2494, 2437, 2382, 2329, 2277, 2226, 2176,
 2128, 2081, 2035, 1990, 1946, 1904, 1862, 1822, 1782, 1744,
 1707
 }; */
uint16 ltc6804gpio_ADC_table[161] =
{
29152,29094,29034,28970,28903,28831,28756,28677,28594,28507,28415,28318,28217,28111,28000,27883, 
27762,27634,27501,27363,27218,27068,26911,26748,26579,26403,26221,26032,25837,25635,25427,25211, 
24990,24761,24526,24285,24037,23783,23522,23256,22983,22705,22421,22132,21838,21539,21235,20927, 
20615,20299,19979,19657,19331,19003,18673,18341,18008,17673,17338,17003,16667,16332,15997,15663, 
15331,15000,14671,14345,14020,13699,13381,13066,12754,12446,12142,11843,11547,11256,10970,10688, 
10411,10139,9872,9610, 9353,9102,8855,8614, 8378,8147,7922,7702,7487,7277,7072,6873, 
6678,6488,6304,6124,5949,5778,5613,5451,5295,5142,4994,4850,4710,4574,4442,4314, 
4189,4068,3951,3837,3727,3620,3516,3415,3317,3222,3130,3041,2954,2870,2789,2710, 
2633,2559,2487,2417,2350,2284,2221,2159,2099,2041,1985,1930,1877,1826,1777,1728, 
1682,1636,1592,1550,1508,1468,1429,1391,1355,1319,1285,1251,1219,1187,1156,1127, 
1098
   };
 /*uint16 ltc6804gpio_ADC_table[181] =
 {
28557,  28466,  28370,  28269,  28162,  28051,  27934,  27812, 
27684,  27550,  27410,  27264,  27111,  26952,  26787,  26615,  
26436,  26250,   26058 ,25858,  25652,  25438,  25217,  24990,  
24755 ,	24513 ,	24265 , 24009,  23747,  23478, 	23203,	22922, 	
22634, 	22340 ,	22041 , 21736 , 21426,  21112, 	20792, 	20469, 	
20141 ,	19810 ,	19475 ,	19138 , 18798,  18456, 	18112, 	17766, 	
17420, 	17073 ,	16726 , 16379 , 16033,  15687, 	15343, 	15000, 	
14659 ,	14321 ,	13985 , 13652,	13322,  12996, 	12673, 	12355, 	
12040 ,	11730 ,	11425 ,	11124, 	10828,  10538, 	10252, 	9972,	  
9697 ,	9427 ,	9164 ,	8905, 	8652,   8405, 	8164, 	7928, 	
7698 ,	7473 ,	7254, 	7040, 	6832,   6629, 	6432, 	6240,	  
6053 ,	5871 ,  5694 ,	5523, 	5356, 	5194,   5036, 	4883,	  
4735 ,	4591 ,	4451 ,	4316, 	4184, 	4057,   3934, 	3814, 	
3698 ,	3585 ,	3476 ,	3371, 	3268, 	3169,   3073, 	2980, 	
2890 ,	2803 ,	2719 ,	2637, 	2558, 	2481,   2407, 	2335, 	
2265 ,	2198 ,	2133 ,	2070, 	2009, 	1949,	  1892, 	1837, 	
1783 ,	1731 ,	1681 ,	1632, 	1585, 	1539,   1495, 	1452, 	
1410 ,	1370 ,	1331 ,	1293, 	1257, 	1221,   1187, 	1154, 	
1121 ,	1090 ,	1060, 	1031, 	1002, 	975,    948,  	922,  	
897 , 	873 ,   849,  	826,  	804, 	  783,  	762, 	  741,    
722 ,   703 , 	684 , 	666,  	649,  	632,  	616, 	  600,    	
584 ,   569 ,   555 , 	540,  	527, 	  513,  	500,  	488,  	
476 , 464 ,	452, 	441,	430
   };*/
/* 
uint16 ltc6804gpio_ADC_table[161]={   //100Kµç×è
29290,29240,29187,29131,29072,29009,28942,28872,28798,28719,
28637,28549,28457,28361,28259,28152,28039,27921,27797,27668,
27532,27390,27241,27086,26925,26756,26581,26399,26209,26012,
25808,25597,25378,25152,24918,24677,24429,24173,23911,23641,
23364,23081,22791,22495,22192,21884,21570,21250,20926,20596,
20263,19925,19584,19239,18892,18542,18190,17837,17482,17127,
16771,16415,16059,15705,15352,15000,14650,14303,13959,13617,
13279,12945,12614,12288,11966,11649,11336,11029,10727,10431,
10139,9854 ,9574 ,9300 ,9032 ,8770 ,8513 ,8263 ,8018 ,7780 ,
7547 ,7321 ,7100 ,6885 ,6675 ,6472 ,6274 ,6081 ,5894 ,5712 ,
5536 ,5364 ,5198 ,5037 ,4880 ,4728 ,4581 ,4438 ,4300 ,4166 ,
4036 ,3911 ,3789 ,3671 ,3557 ,3447 ,3340 ,3236 ,3136 ,3039 ,
2945 ,2855 ,2767 ,2682 ,2600 ,2520 ,2444 ,2369 ,2297 ,2228 ,
2160 ,2095 ,2032 ,1971 ,1912 ,1855 ,1800 ,1747 ,1695 ,1645 ,
1597 ,1550 ,1505 ,1461 ,1419 ,1378 ,1338 ,1300 ,1263 ,1227 ,
1192 ,1158 ,1125 ,1094 ,1063 ,1034 ,1005 ,977  , 950 ,924  ,
899
} ; */
 
	
	
	uint8 i,low = 0;
	uint16 temp1,temp2;
	uint8 high = sizeof(ltc6804gpio_ADC_table)/sizeof(uint16) - 1;          // 160
	uint8 mid = (low + high)/2;                                             // 80
	int8 t;                                                         
	
	
	if((adc > ltc6804gpio_ADC_table[low]) || (adc < ltc6804gpio_ADC_table[high])) 
	{
	    return 0;   
	}
	else
	{
		for(i = 0; i < 160; i++)
		{
  			mid = (low + high) / 2;                                    // 80
  			if (adc == ltc6804gpio_ADC_table[mid])
  			{
  			    t = mid;
  			    t = t - 40;
  				  return t;
  			}
  			else
  			{	
    				if(adc < ltc6804gpio_ADC_table[mid]) 
    				{
    				    low = mid + 1;
    				}
    				else 
    				{
    				    high = mid - 1;  
    				}
    				if((adc < ltc6804gpio_ADC_table[mid]) && (adc > ltc6804gpio_ADC_table[mid + 1]))
    				{   
      				  temp1 = adc - ltc6804gpio_ADC_table[mid+1];
      				  temp2 = ltc6804gpio_ADC_table[mid] - ltc6804gpio_ADC_table[mid+1];
      				  temp2 = temp2/2; 
      				  	
      					if( temp1 >= temp2 ) 
      					{
      					    t = mid  + 1; 
      					} 
      					else
      					{
      					    t = mid;
      					}
      					t = t - 40 ;
      					return t;
    				}
  			}
		}
	}
	return t;
}

/*
===============================================================================
 Name        : 16_Day39_11102021_ADC001.c

 Description : Program demonstrate ADC Polling method software controlled
 	 	 	   start of conversion burst mode disabled and displaying on LCD

 
 Layered Architecture used for this project
 ************************************
 Application layer-16_Day39_11102021_ADC001.c
 ************************************
 Board layer - lcd.c/.h, configboard.h
 ************************************
 Low level drivers or chip level - pinmux.c/.h,adc.c/.h, gpio.c/.h
 ************************************
 Hardware
 ************************************

===============================================================================
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
#include "pinmux.h"
#include "adc.h"
#include "lcd.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MAXTIMEOUT 		0xFFFFF
#define CLEARTIMEOUT	0x3FFFF
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  Initialize all the hardware connected
  * @retval none
  */
void vAppHardwareInit(void)
{
	vPinmuxInitialize();
	vLCDInitialize();
	vLCDInitialCommands();
	vADCInitialize();

}

/**
  * @brief  Crude Delay
  * @retval none
  */
void vAppDelay(uint32_t count)
{
	int i,j;
	for(i=0;i<count;i++)
		for(j=0;j<0xA00;j++);
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
	uint32_t delaycount=0,readADCGDRdata=0,channelno=0,adcresult=0;

  /* Initialize all configured peripherals */
   vAppHardwareInit();

   /* Disabling the burst mode as we are going to use software controlled starting mechanism */
   vADCBurstMode(BURSTDISABLE);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)  // for(;;)
  {

  delaycount++;
		switch (delaycount) {
		case CLEARTIMEOUT:
		vLCDClear();
			break;
		case MAXTIMEOUT:
			/* Select the channel no 0 for conversion */
			vADCChannelSel(ADCCHAN0);

			/* Start the conversion now */
			vADCStartConv(START_NOW);

			/* Wait until ADC conversion completes for the selected channel and by setting the DONE bit */
			while(!(LPC_ADC->ADGDR& 1<< BIT_AD0GDR_DONE));

			/* Reading the global data register into a variable */
			readADCGDRdata = LPC_ADC->ADGDR;

			/* Extracting the channel no from the global data register which is in 26:24 bits */
			channelno = (readADCGDRdata>>24) & 0x07;

			/* Extracting the adc result from the global data register which is in 15:4 bits */
			adcresult = (readADCGDRdata>>4)  & 0xFFF;

			/* Displaying the result on the LCD */
			vLCDGotoxy(LINE0,COL0);
			vLCDPrintf("A[%d]=%d",channelno,adcresult);
			delaycount=0;
			break;

		default:
			/* DO NOTHING */
			break;
		}







  }
  /* End of Application entry point */
}





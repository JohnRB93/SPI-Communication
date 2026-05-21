/* x = value, y = bit to check */
#define CHECK_BIT(x, y) (((x) >> (y)) & 0x1)
/* x = value, y = bit to set */
#define BIT_SET(x, y) ((x) |= (0x1 << (y)))
/* x = value, y = bit to clear */
#define BIT_CLEAR(x, y) ((x) &= ~(0x1 << (y)))
/* x = value, y = bit to toggle */
#define BIT_TOGGLE(x, y) ((x) ^= (0x1 << (y)))

/*
 * This program demonstrates SPI communication with an arduino board; STM32 board as master and the
 * arduino as slave. The STM32 board is connected to four push buttons and the arduino through a
 * voltage level converter module. The arduino is connected to four LEDs that correspond to the STM's
 * push buttons. When a push button is pressed, the STM32 sends data to the arduino via SPI peripheral
 * to turn on the corresponding LED. The press of the button causes an Interrupt Service Routine to send
 * the data. The baud rate for the SPI communication is 500kHz.
 */

/*
 * SPI2 Pin Configurations:
 * 	PB12 -> NSS
 * 	PB13 -> SCK
 * 	PB14 -> MISO
 * 	PB15 -> MOSI
 *
 * Push Button Pins:
 *  PC00 -> Red Push Button
 *  PC01 -> Yellow Push Button
 *  PC02 -> Green Push Button
 *  PC03 -> Blue Push Button
 */

#include<stdint.h>
#include<string.h>
#include"spi.h"
#include"gpio.h"
#include"irq.h"
#include"tim.h"

/************ Global Variables and Macros *********************************************************************/
#define NSS				GPIO_PIN_NO_12
#define CLK				GPIO_PIN_NO_13
#define MISO			GPIO_PIN_NO_14
#define MOSI			GPIO_PIN_NO_15

#define RED_BTN			GPIO_PIN_NO_4
#define YELLOW_BTN		GPIO_PIN_NO_1
#define GREEN_BTN		GPIO_PIN_NO_2
#define BLUE_BTN		GPIO_PIN_NO_3

#define RED_BTN_PRESSED			10
#define YELLOW_BTN_PRESSED		15
#define GREEN_BTN_PRESSED		20
#define BLUE_BTN_PRESSED		25

uint8_t btnPressed;
/**************************************************************************************************************/

/************ Function Headers ********************************************************************************/
void setUpSpi(void);
void setUpButtons(void);
/**************************************************************************************************************/

/************ Main Function ***********************************************************************************/
int main(void)
{
	setUpButtons();
	setUpSpi();
	TIM_Init(TIM2);
	btnPressed = 0;
	uint8_t dataTx;
	while(1)
	{
		if(btnPressed == RED_BTN_PRESSED)
		{
			dataTx = 'r';
			GPIO_WriteToOutputPin(GPIOB, NSS, RESET);
			SPI_MasterTransmissionStartTx(SPI2, &dataTx);
			GPIO_WriteToOutputPin(GPIOB, NSS, SET);
			btnPressed = 0;
		}
		else if(btnPressed == YELLOW_BTN_PRESSED)
		{
			dataTx = 'y';
			GPIO_WriteToOutputPin(GPIOB, NSS, RESET);
			SPI_MasterTransmissionStartTx(SPI2, &dataTx);
			GPIO_WriteToOutputPin(GPIOB, NSS, SET);
			btnPressed = 0;
		}
		else if(btnPressed == GREEN_BTN_PRESSED)
		{
			dataTx = 'g';
			GPIO_WriteToOutputPin(GPIOB, NSS, RESET);
			SPI_MasterTransmissionStartTx(SPI2, &dataTx);
			GPIO_WriteToOutputPin(GPIOB, NSS, SET);
			btnPressed = 0;
		}
		else if(btnPressed == BLUE_BTN_PRESSED)
		{
			dataTx = 'b';
			GPIO_WriteToOutputPin(GPIOB, NSS, RESET);
			SPI_MasterTransmissionStartTx(SPI2, &dataTx);
			GPIO_WriteToOutputPin(GPIOB, NSS, SET);
			btnPressed = 0;
		}
	}
}
/**************************************************************************************************************/

/************ Function Definitions ****************************************************************************/
void setUpSpi(void)
{
	//Configure GPIO pins for SPI2
	GPIO_Init(GPIOB);
	//NSS
	GPIO_SetPinMode(GPIOB, NSS, GPIO_MODE_OUT);
	GPIO_SetPinSpeed(GPIOB, NSS, GPIO_SPEED_HIGH);
	GPIO_WriteToOutputPin(GPIOB, NSS, SET);
	//CLK
	GPIO_SetPinMode(GPIOB, CLK, GPIO_MODE_ALTFN);
	GPIO_SetPinSpeed(GPIOB, CLK, GPIO_SPEED_HIGH);
	GPIO_SetAlternateConfig(GPIOB, CLK, GPIO_AF_05);
	//MOSI
	GPIO_SetPinMode(GPIOB, MOSI, GPIO_MODE_ALTFN);
	GPIO_SetPinSpeed(GPIOB, MOSI, GPIO_SPEED_HIGH);
	GPIO_SetAlternateConfig(GPIOB, MOSI, GPIO_AF_05);

	//Initialize SPI2 Peripheral
	SPI_Init(SPI2);
	//Set Baud Rate.
	SPI_SetBaudRateDiv(SPI2, SPI_BAUD_DIV_32);  /* Baud Rate = 16MHz/32 = 500kHz */
	//Select the CPOL and CPHA bits to define one of the four relationships between the data transfer and the serial clock.
	SPI_SetClkRelationship(SPI2, SPI_CLOCK_POL_PHA_1);
	//Set the DFF bit to define 8- or 16-bit data frame format.
	SPI_SetDataBitMode(SPI2, SPI_8BIT_DATA_FRAME);
	//Define the frame format (MSB or LSB first).
	SPI_SetFrameFormat(SPI2, SPI_MSB_TRANSMIT_FIRST);
	//Configure slave management.
	SPI_SetSlaveManagement(SPI2, SPI_SWS_MGNT);
	//Set the FRF bit in SPI_CR2 to select the TI or Motorola protocol for serial communications.
	SPI_SetProtocol(SPI2, SPI_MOTOROLA_MODE);
	//Configure the directional data mode.
	SPI_SetDirectionMode(SPI2, SPI_1LINE_BIDRECTIONAL_TX_ONLY_MODE);
	//The MSTR and SPE bits must be set (they remain set only if the NSS pin is connected to a high-level signal).
	SPI_SetMasterMode(SPI2);
}

void setUpButtons(void)
{
	GPIO_Init(GPIOC);
	GPIO_EnableInterrupts(GPIOC, RED_BTN, GPIO_MODE_IT_RT);
	GPIO_EnableInterrupts(GPIOC, YELLOW_BTN, GPIO_MODE_IT_RT);
	GPIO_EnableInterrupts(GPIOC, GREEN_BTN, GPIO_MODE_IT_RT);
	GPIO_EnableInterrupts(GPIOC, BLUE_BTN, GPIO_MODE_IT_RT);
	IRQInterruptConfig(EXTI4_IRQn, ENABLE);
	IRQInterruptConfig(EXTI1_IRQn, ENABLE);
	IRQInterruptConfig(EXTI2_IRQn, ENABLE);
	IRQInterruptConfig(EXTI3_IRQn, ENABLE);
	// Make sure the Pull-up/Pull-down registers stay in place.
	GPIOC->PUPDR |= (0x2A8 << 0);
}
/**************************************************************************************************************/

/************ IRQ Handling and Application Event Callbacks ****************************************************/

void EXTI1_IRQHandler(void)
{
	TIM_Delay_ms(TIM2, 50);
	EXTI1_IRQHandling();
}

void EXTI2_IRQHandler(void)
{
	TIM_Delay_ms(TIM2, 50);
	EXTI2_IRQHandling();
}

void EXTI3_IRQHandler(void)
{
	TIM_Delay_ms(TIM2, 50);
	EXTI3_IRQHandling();
}

void EXTI4_IRQHandler(void)
{
	TIM_Delay_ms(TIM2, 50);
	EXTI4_IRQHandling();
}

extern void GPIO_ApplicationEventCallBack(uint8_t pinNumber)
{
	if(pinNumber == RED_BTN)
		btnPressed = RED_BTN_PRESSED;
	else if(pinNumber == YELLOW_BTN)
		btnPressed = YELLOW_BTN_PRESSED;
	else if(pinNumber == GREEN_BTN)
		btnPressed = GREEN_BTN_PRESSED;
	else if(pinNumber == BLUE_BTN)
		btnPressed = BLUE_BTN_PRESSED;
}
/**************************************************************************************************************/

#include"spi.h"
#include"irq.h"

/***************** User Application Exposed Function Definitions ***********************/

/*
 * @fn			- SPI_Init
 *
 * @brief		- This function initializes the SPI peripheral in Master Mode
 * 				  	with user-provided configurations.
 * 				  Each pin that is to be used for SPI communication (except NSS if
 * 				    using Software Slave Management) must be configured to alternate
 * 				    functionality mode first before calling this function.
 * 				  The Peripheral-Clock division is used to determine the baudrate
 * 				  	for the SPI peripheral.
 *
 * @param[in]	- SPI base address.
 *
 * @return		- None.
 *
 * @note		- It is recommended to always enable and use interrupts when
 * 				  	using the SPI peripheral to reduce CPU load with polling.
 * 				  If using Software Slave Management, the NSS must be set to HIGH.
 * 				  In this configuration the MOSI pin is a data output and the MISO pin is a data input.
 */
void SPI_Init(SPI_TypeDef *SPIx)
{
	if(SPIx == SPI1)
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	else if(SPIx == SPI2)
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	else if(SPIx == SPI3)
		RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
}

/*
 * @fn			- SPI_DeInit
 *
 * @brief		- This function disables the SPI peripheral.
 *
 * @param[in]	- SPI base address.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_DeInit(SPI_TypeDef *SPIx)
{
	if(SPIx == SPI1)
		RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
	else if(SPIx == SPI2)
		RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
	else if(SPIx == SPI3)
		RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;
}

/*
 * @fn			- SPI_SetBaudRateDiv
 *
 * @brief		- This function sets the clock SPI peripheral clock
 * 				  division for the baud rate.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Division factor
 * 				  (SPI_BAUD_DIV_2, SPI_BAUD_DIV_4, ... , SPI_BAUD_DIV_256).
 *
 * @return		- None.
 *
 * @note		- Baud Rate = Peripheral Clock / Clock Division.
 */
void SPI_SetBaudRateDiv(SPI_TypeDef *SPIx, uint8_t div)
{
	if(div == SPI_BAUD_DIV_2)
		SPIx->CR1 &= ~SPI_CR1_BR;
	else
	{
		SPIx->CR1 &= ~SPI_CR1_BR;
		SPIx->CR1 |= (div << SPI_CR1_BR_Pos);
	}
}

/*
 * @fn			- SPI_SetClkRelationship
 *
 * @brief		- This function sets the clock phase/polarity relationship.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Clock phase/polarity mode.
 *
 * @return		- None.
 *
 * @note		- SPI_CLOCK_POL_PHA_0
 * 				  (Clock low idle and data capture of first bit on first edge (rising edge) [CPOL = 0, CPHA = 0])
 *
 * 				  SPI_CLOCK_POL_PHA_1
 * 				  (Clock high idle and data capture of first bit on first edge (falling edge) [CPOL = 1, CPHA = 0])
 *
 * 				  SPI_CLOCK_POL_PHA_2
 * 				  (Clock low idle and data capture of first bit on second edge (falling edge) [CPOL = 0, CPHA = 1])
 *
 * 				  SPI_CLOCK_POL_PHA_3
 * 				  (Clock high idle and data capture of first bit on second edge (rising edge) [CPOL = 1, CPHA = 1])
 */
void SPI_SetClkRelationship(SPI_TypeDef *SPIx, uint8_t clkReshp)
{
	if(clkReshp == SPI_CLOCK_POL_PHA_0)
	{
		SPIx->CR1 &= ~SPI_CR1_CPOL;
		SPIx->CR1 &= ~SPI_CR1_CPHA;
	}
	else if(clkReshp == SPI_CLOCK_POL_PHA_1)
	{
		SPIx->CR1 |= SPI_CR1_CPOL;
		SPIx->CR1 &= ~SPI_CR1_CPHA;
	}
	else if(clkReshp == SPI_CLOCK_POL_PHA_2)
	{
		SPIx->CR1 &= ~SPI_CR1_CPOL;
		SPIx->CR1 |= SPI_CR1_CPHA;
	}
	else if(clkReshp == SPI_CLOCK_POL_PHA_3)
	{
		SPIx->CR1 |= SPI_CR1_CPOL;
		SPIx->CR1 |= SPI_CR1_CPHA;
	}
}

/*
 * @fn			- SPI_SetDataBitMode
 *
 * @brief		- This function sets the DFF bit to define 8-bit or 16-bit data frame format
 * 				  for transmission/reception.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Data Bit Mode (SPI_8BIT_DATA_FRAME, SPI_16BIT_DATA_FRAME).
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_SetDataBitMode(SPI_TypeDef *SPIx, uint8_t dataBitMode)
{
	if(dataBitMode == SPI_8BIT_DATA_FRAME)
		SPIx->CR1 &= ~SPI_CR1_DFF;
	else
		SPIx->CR1 |= SPI_CR1_DFF;
}

/*
 * @fn			- SPI_SetFrameFormat
 *
 * @brief		- This function sets the frame format to transmit either the
 * 				  LSB or MSB first.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Frame Format (SPI_MSB_TRANSMIT_FIRST, SPI_LSB_TRANSMIT_FIRST).
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_SetFrameFormat(SPI_TypeDef *SPIx, uint8_t frameFormat)
{
	if(frameFormat == SPI_MSB_TRANSMIT_FIRST)
		SPIx->CR1 &= ~SPI_CR1_LSBFIRST;
	else
		SPIx->CR1 |= SPI_CR1_LSBFIRST;
}

/*
 * @fn			- SPI_SetSlaveManagement
 *
 * @brief		- This function sets the configuration for slave device
 * 				  management.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Slave management configuration.
 * 				  (SPI_HWS_MGNT_NSS_OP_ENABLED, SPI_HWS_MGNT_NSS_OP_DISABLED,
 * 				  SPI_SWS_MGNT)
 *
 * @return		- None.
 *
 * @note		- SPI_HWS_MGNT_NSS_OP_ENABLED
 * 				  	- NSS output enabled (SSM = 0, SSOE = 1)
 * 				  	- This configuration is used only when the device operates in master mode.
 * 				  	- The NSS signal is driven low when the master starts the communication and
 * 				  	  is kept low until the SPI is disabled.
 * 				- SPI_HWS_MGNT_NSS_OP_DISABLED
 * 					- NSS output disabled (SSM = 0, SSOE = 0)
 * 					- This configuration allows multimaster capability for devices operating in
 * 					  master mode.
 * 					- For devices set as slave, the NSS pin acts as a classical NSS input: the
 * 					  slave is selected when NSS is low and deselected when NSS high.
 * 				- SPI_SWS_MGNT
 * 					- Software NSS management.
 */
void SPI_SetSlaveManagement(SPI_TypeDef *SPIx, uint8_t slaveMgmt)
{
	if(slaveMgmt == SPI_HWS_MGNT_NSS_OP_ENABLED)
	{
		SPIx->CR1 &= ~SPI_CR1_SSM;
		SPIx->CR2 |= SPI_CR2_SSOE;
	}
	else if(slaveMgmt == SPI_HWS_MGNT_NSS_OP_DISABLED)
	{
		SPIx->CR1 &= ~SPI_CR1_SSM;
		SPIx->CR2 &= ~SPI_CR2_SSOE;
	}
	else if(slaveMgmt == SPI_SWS_MGNT)
	{
		SPIx->CR1 |= SPI_CR1_SSM;
		SPIx->CR1 |= SPI_CR1_SSI;
	}
}

/*
 * @fn			- SPI_SetProtocol
 *
 * @brief		- This function sets the protocol to either TI or Motorola mode.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Protocol Mode (SPI_MOTOROLA_MODE, SPI_TI_MODE).
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_SetProtocol(SPI_TypeDef *SPIx, uint8_t protocol)
{
	if(protocol == SPI_MOTOROLA_MODE)
		SPIx->CR2 &= ~SPI_CR2_FRF;
	else
		SPIx->CR2 |= SPI_CR2_FRF;
}

/*
 * @fn			- SPI_SetDirectionMode
 *
 * @brief		- This function sets the full/half duplex and direction
 * 				  of communication.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Direction and Full/Half Duplex configuration.
 * 				  	- SPI_2LINE_UNIDIRECTIONAL_RX_ONLY_MODE --> Unidirectional Receive-Only Mode
 * 				  	- SPI_2LINE_UNIDIRECTIONAL_RX_TX_MODE ----> Full Duplex Mode
 * 				  	- SPI_1LINE_BIDRECTIONAL_RX_ONLY_MODE ----> Bidirectional Receive-Only Mode
 * 				  	- SPI_1LINE_BIDRECTIONAL_TX_ONLY_MODE ----> Bidirectional Transmit-Only Mode
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_SetDirectionMode(SPI_TypeDef *SPIx, uint8_t direction)
{
	if(direction == SPI_2LINE_UNIDIRECTIONAL_RX_ONLY_MODE)
	{	//Unidirectional Receive-Only Mode.
		SPIx->CR1 &= ~SPI_CR1_BIDIMODE;
		SPIx->CR1 |= SPI_CR1_RXONLY;
	}
	else if(direction == SPI_2LINE_UNIDIRECTIONAL_RX_TX_MODE)
	{	//Full Duplex Mode.
		SPIx->CR1 &= ~SPI_CR1_BIDIMODE;
		SPIx->CR1 &= ~SPI_CR1_RXONLY;
	}
	else if(direction == SPI_1LINE_BIDRECTIONAL_RX_ONLY_MODE)
	{	//Bidirectional Receive-Only Mode.
		SPIx->CR1 |= SPI_CR1_BIDIMODE;
		SPIx->CR1 &= ~SPI_CR1_BIDIOE;
	}
	else if(direction == SPI_1LINE_BIDRECTIONAL_TX_ONLY_MODE)
	{	//Bidirectional Transmit-Only Mode.
		SPIx->CR1 |= SPI_CR1_BIDIMODE;
		SPIx->CR1 |= SPI_CR1_BIDIOE;
	}
}

/*
 * @fn			- SPI_SetMasterMode
 *
 * @brief		- This function sets this device in master mode.
 *
 * @param[in]	- SPI base address.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_SetMasterMode(SPI_TypeDef *SPIx)
{
	SPIx->CR1 |= SPI_CR1_MSTR;
}

/*
 * @fn			- SPI_SetSlaveMode
 *
 * @brief		- This function sets this device in slave mode.
 *
 * @param[in]	- SPI base address.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_SetSlaveMode(SPI_TypeDef *SPIx)
{
	SPIx->CR1 &= ~SPI_CR1_MSTR;
}

/*
 * @fn			- SPI_MasterTransmissionStartTx
 *
 * @brief		- This function begins the data transmission process for
 * 				  master mode SPI communication. Only transmits data to
 * 				  the slave device.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Pointer to data to transmit.
 *
 * @return		- None.
 *
 * @note		- Bidirectional Transmit-Only Mode.
 */
void SPI_MasterTransmissionStartTx(SPI_TypeDef *SPIx, uint8_t *dataTx)
{
	uint8_t* p = dataTx;
	//Enable the SPI by setting the SPE bit to 1.
	SPIx->CR1 |= SPI_CR1_SPE;
	//Write the first data item to send into the SPI_DR register (this clears the TXE bit).
	do{ //Wait until TXE=1 and write the next data item to be transmitted.
		while(!((SPIx->SR >> SPI_SR_TXE_Pos) & 0x1U)){}
		SPIx->DR = *p;
		p++;  //Repeat this step for each data item to be transmitted.
	}while(*p != '\0');
	//After writing the last data item into the SPI_DR register, wait until TXE=1,
	//then wait until BSY=0, this indicates that the transmission of the last data is complete.
	while(!((SPIx->SR >> SPI_SR_TXE_Pos) & 0x1U)){}
	while(((SPIx->SR >> SPI_SR_BSY_Pos) & 0x1U)){}
	SPIx->CR1 &= ~SPI_CR1_SPE;
}

/*
 * @fn			- SPI_MasterTransmissionStartRx
 *
 * @brief		- This function begins the data transmission process for
 * 				  master mode SPI communication. Only receives data from
 * 				  the slave device.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Pointer to data to be received.
 *
 * @return		- None.
 *
 * @note		- Bidirectional Receive-Only Mode.
 */
void SPI_MasterTransmissionStartRx(SPI_TypeDef *SPIx, uint8_t *dataRx)
{

}

/*
 * @fn			- SPI_MasterTransmissionStartTxRx
 *
 * @brief		- This function begins the data transmission process for
 * 				  master mode SPI communication. Transmits and receives
 * 				  data to and from the slave device.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Pointer to data to transmit.
 * @param[in]	- Pointer to data to be received.
 *
 * @return		- None.
 *
 * @note		- Unidirectional Transmit and Receive Mode (Full Duplex).
 */
void SPI_MasterTransmissionStartTxRx(SPI_TypeDef *SPIx, uint8_t *dataTx, uint8_t *dataRx, uint8_t rxLength)
{
	//Define local variables.
	uint8_t* p = dataTx;
	uint8_t dataRxInc = 0;
	//Enable the SPI by setting the SPE bit to 1.
	SPIx->CR1 |= SPI_CR1_SPE;
	//Write the first data item to be transmitted into the SPI_DR register (this clears the TXE flag).
	SPIx->DR = *p;
	p++;
	/*
	 Wait until TXE=1 and write the second data item to be transmitted. Then wait until
	 RXNE=1 and read the SPI_DR to get the first received data item (this clears the RXNE
	 bit). Repeat this operation for each data item to be transmitted/received until the n–1
	 received data.
	 */
	//for(uint16_t i = 0; i < 1000; i++){}
	do{
	while(SPI_TX_EMPTY){}
	if(*p == '\0')	//If there's no more data to send, but still receiving data, keep the MOSI line high.
		SPIx->DR = 0xFF;
	else
	{
		SPIx->DR = *p;
		p++;
	}

	while(SPI_RX_NOT_EMPTY){}
	//for(uint16_t i = 0; i < 500; i++){}
	*dataRx = SPIx->DR;
	dataRx++;
	dataRxInc++;

	}while(dataRxInc < rxLength - 1);

	//Wait until RXNE=1 and read the last received data.
	while(SPI_RX_NOT_EMPTY){}
	*dataRx = SPIx->DR;
	//Wait until TXE=1 and then wait until BSY=0 before disabling the SPI.
	while(SPI_TX_EMPTY){}
	while(SPI_BUSY_FLAG){}
	SPIx->CR1 &= ~SPI_CR1_SPE;
	dataRx -= dataRxInc;
}

/*
 * @fn			- SPI_SlaveTransmissionStartTx
 *
 * @brief		- This function begins the data transmission process for
 * 				  master mode SPI communication. Only transmits data to
 * 				  the slave device.
 *
 * @param[in]	- SPI base address.
 * @param[in]		- Data to transmit.
 *
 * @return		- None.
 *
 * @note		- Bidirectional Transmit-Only Mode.
 */
void SPI_SlaveTransmissionStartTx(SPI_TypeDef *SPIx, uint8_t *dataTx)
{

}

/*
 * @fn			- SPI_SlaveTransmissionStartRx
 *
 * @brief		- This function begins the data transmission process for
 * 				  master mode SPI communication. Only receives data from
 * 				  the slave device.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Data to be received.
 *
 * @return		- None.
 *
 * @note		- Bidirectional Receive-Only Mode.
 */
void SPI_SlaveTransmissionStartRx(SPI_TypeDef *SPIx, uint8_t *dataRx)
{

}

/*
 * @fn			- SPI_SlaveTransmissionStartTxRx
 *
 * @brief		- This function begins the data transmission process for
 * 				  master mode SPI communication. Transmits and receives
 * 				  data to and from the slave device.
 *
 * @param[in]	- SPI base address.
 * @param[in]	- Data to transmit.
 * @param[in]	- Data to be received.
 *
 * @return		- None.
 *
 * @note		- Unidirectional Transmit and Receive Mode (Full Duplex).
 */
void SPI_SlaveTransmissionStartTxRx(SPI_TypeDef *SPIx, uint8_t *dataTx, uint8_t *dataRx)
{

}

/*
 * @fn			- SPI_NSS_Enable
 *
 * @brief		- This function enables the slave device by setting the NSS
 * 				  pin to LOW.
 *
 * @param[in]	- GPIOx base address.
 * @param[in]	- GPIO pin number.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_NSS_Enable(GPIO_TypeDef *GPIOx, uint8_t gpioPin)
{
	GPIOx->ODR &= ~(1U << gpioPin);
}

/*
 * @fn			- SPI_NSS_Disable
 *
 * @brief		- This function disables the slave device by setting the NSS
 * 				  pin to HIGH.
 *
 * @param[in]	- GPIOx base address.
 * @param[in]	- GPIO pin number.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_NSS_Disable(GPIO_TypeDef *GPIOx, uint8_t gpioPin)
{
	GPIOx->ODR |= (1U << gpioPin);
}

/*
 * @fn			- SPI_EnableInterrupts
 *
 * @brief		- This function enables the SPI interrupts.
 *
 * @param[in]	- SPIx base address.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_EnableInterrupts(SPI_TypeDef *SPIx)
{
	SPIx->CR2 |= SPI_CR2_TXEIE;
	SPIx->CR2 |= SPI_CR2_RXNEIE;
	SPIx->CR2 |= SPI_CR2_ERRIE;

	if(SPIx == SPI1)
		IRQInterruptConfig(SPI1_IRQn, ENABLE);
	else if(SPIx == SPI2)
		IRQInterruptConfig(SPI1_IRQn, ENABLE);
	else if(SPIx == SPI3)
		IRQInterruptConfig(SPI1_IRQn, ENABLE);
}

/*
 * @fn			- SPI_EnableDMA
 *
 * @brief		- This function enables the SPI DMA.
 *
 * @param[in]	- SPIx base address.
 *
 * @return		- None.
 *
 * @note		- None.
 */
void SPI_EnableDMA(SPI_TypeDef *SPIx)
{
	SPIx->CR2 |= SPI_CR2_TXDMAEN;
	SPIx->CR2 |= SPI_CR2_RXDMAEN;
}


/***************************************************************************************/


/***************** SPI IRQ Handling ***************************************************/

/* Weak function that can be implemented in user application. */
void __attribute__((weak)) SPI_ApplicationEventCallback(void){}

/*
 * @fn			- SPI_IRQHandling
 *
 * @brief		- This function handles an SPI interrupt.
 *
 * @param[SPI_TypeDef]	- Base Address of the SPI Peripheral.
 *
 * @return		- None.
 *
 * @note		- The interrupt bits(TXEIE, RXNEIE, or ERRIE) must
 * 				  be set in order for an interrupt to be triggered.
 */
void SPI_IRQHandling(SPI_TypeDef *SPIx)
{
	SPI_ApplicationEventCallback();
}


/***************************************************************************************/

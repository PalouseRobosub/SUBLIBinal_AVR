
#include "SPI.h"


//Copyright 2015 Palouse RoboSub Club

/*
  This file is part of Sublibinal.

    Sublibinal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sublibinal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sublibinal.  If not, see <http://www.gnu.org/licenses/>.
 */


//callback functions
void (*spi_1_tx_callback) (void);
void (*spi_1_rx_callback) (void);

void (*spi_2_tx_callback) (void);
void (*spi_2_rx_callback) (void);


SPI_Data spi_1;
SPI_Data spi_2;

SPI_Data* initialize_SPI(SPI_Config config) {

    /*
    1. If using interrupts:
        a) Clear the SPIxIF bit in the respective IFSx register.
        b) Select an interrupt mode using the SISEL<2:0> bits (SPIxSTAT<4:2>).
        c) Set the SPIxIE bit in the respective IECx register.
        d) Write the SPIxIP bits in the respective IPCx register.
    2. When MSTEN (SPIxCON1<5>) = 1, write the desired settings to the SPIxCON1 and SPIxCON2 registers.
    3. Clear the SPIROV bit (SPIxSTAT<6>).
    4. Select Enhanced Buffer mode by setting the SPIBEN bit (SPIxCON2<0>).
    5. Enable the SPIx operation by setting the SPIEN bit (SPIxSTAT<15>).
    6. Write the data to be transmitted to the SPIxBUF register. The transmission (and reception) starts as soon as data is written to the SPIxBUF register.
     */

    //setup the rx and tx buffers
    spi_1.Rx_queue = create_queue(config.rx_buffer_ptr, config.rx_buffer_size);
    spi_1.Tx_queue = create_queue(config.tx_buffer_ptr, config.tx_buffer_size);

    spi_1_tx_callback = config.tx_callback;
    spi_1_rx_callback = config.rx_callback;

	//Set up the clock based on the clock divider
	SPCR = (SPRC)&~(0b11) | (config.divider&(~0b11));
	if (config.divider >= 0b100)
		SPSR |= 1; //Set the 2x bit

	SPCR |= 1<<4; //Enable master mode
	
	if (config.clk_edge)
		SPCR |= 1<<CPOL; //Enable the falling edge as leading edge

	//Always sample on leading edge?
	if (config.clk_phase)
		SPCR |= 1<<CPHA; //Enable setup on leading edge

	//always transmit MSB first

	//Configure the SS pin as an output! This avoids bus contention
	DDRB |= 1<<2;

	SPCR |= config.enable<<SPIE; //enable SPI interrupts
	SPCR |= config.enable<<SPE; //enable SPI

    spi_1.is_idle = TRUE;

    return &spi_1;
}

Error send_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size) {
    Error status;
	uint8 transmission;
    //we need to place the provided data onto the Tx queue
    status = enqueue(spi_1.Rx_queue, data_ptr, data_size);
	if (status == ERR_NO_ERR && spi_1.is_idle == TRUE)
	{
		status = dequeue(spi_1.Tx_queue, &transmission, sizeof(transmission));
		if (status == ERR_NO_ERR)
			SPDR = transmission;
		spi_1.is_idle = FALSE;
	}
	
	return status;
}

//int receive_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size) {
//    int status;
//    //we need to read the specified data from the rx queue
//    switch (channel) {
//        case SPI_CH_1:
//            status = dequeue(&(spi_1.Rx_queue), data_ptr, data_size);
//            break;
//        case SPI_CH_2:
//            status = dequeue(&(spi_2.Rx_queue), data_ptr, data_size);
//            break;
//        default:
//            status = 1; //return failure
//            break;
//    }
//    return status; //return success
//}

ISR(SPI_STC_vect) {
	uint8 transmit;

	//check for more data in the queue
	if (dequeue(spi_1.Tx_queue, &transmit, sizeof(transmit)) == ERR_NO_ERR)
		//if data is available, load it into the buffer
		SPDR = transmit;
	//if no data is availble, set the bus as idle
	else
		spi_1.is_idle = TRUE;
}


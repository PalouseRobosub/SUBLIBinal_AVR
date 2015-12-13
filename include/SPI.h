/*!
 * \file SPI.h
 *
 * Defines the data structures and methods for using the SPI channels
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"
#include "Queue.h"

	/*Enums*/
	typedef struct SPI_CLK_DIV {
		Div_4 = 0,
		Div_16 = 1,
		Div_64 = 0b010,
		Div_128 = 0b011,
		Div_2 = 0b100,
		Div_8 = 0b101,
		Div_32 = 0b110
	} SPI_clk_div;

    /*Object Defintions*/
    typedef struct SPI_DATA {
        Queue Rx_queue;///< Received data
        Queue Tx_queue;///< Data that needs to be sent
        boolean is_idle;///< Idle information
    } SPI_Data;///< Object for references to the data queues

    typedef struct SPI_CONFIG {
        SPI_clk_div divider; ///<Clock divider to use for SCK frequency from the system clock
		uint8 clk_edge; ///< Define leading edge as Rising by setting 0, Set leading edge to falling with 1
		uint8 clk_phase;///<Defne the leading edge as sample with 0, Setup with 1
        uint8 *tx_buffer_ptr; ///< Pointer to the transmit buffer queue
        uint tx_buffer_size; ///< Size of the transmit buffer
        void (*tx_callback); ///< Function to call when transmission is made
        uint8 *rx_buffer_ptr; ///< Pointer to the receive buffer queue
        uint rx_buffer_size; ///< Size of the receive buffer
        void (*rx_callback); ///< Function to call when data is received
        boolean enable; ///< Enable SPI
    }SPI_Config; ///< Configuration struct for initializing an SPI channel

    /**
     * Initialize an SPI channel
     * \param config The struct containing the configuration to apply to the channel
     * \see SPI_CONFIG
     * \return Pointer to the transmit and receive queue
     */
    SPI_Data* initialize_SPI(SPI_Config config);

    /**
     * Send data on the SPI bus
     * \param channel The channel to send on
     * \param data_ptr The data to send
     * \param data_size The size of the data
     * \return Error type
     * \see Error
     */
    Error send_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size);

    //int receive_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size);




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */


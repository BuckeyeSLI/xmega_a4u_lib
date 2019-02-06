/*
 * Name    : xa4u_usart.h
 * Created : 2/6/2019 1:26:47 AM
 * Author  : Andrew Bennett
 * 
 * Description : Implements a FIFO buffer for interrupt driven USARTs. Does not set up the USARTs, this must be done in main.c or with another library.
 */ 

#ifndef XA4U_USART_H
#define XA4U_USART_H

#include <util/atomic.h>

// USART I/O buffer length - define in main.c to override
#ifndef USART_BUFFER_SIZE
#define USART_BUFFER_SIZE 128
#endif

// FIFO Buffer struct definition - used to construct a single FIFO buffer
typedef struct FIFOBuffer_struct
{
	uint8_t buffer[USART_BUFFER_SIZE];
	uint8_t buffer_head;
	uint8_t buffer_tail;
	uint8_t buffer_size;
} FIFOBuffer_t;

// USART enum definition - used to select which USART a method is targeted at
typedef enum XA4U_USART_enum
{
	XA4U_USART_C0,
	XA4U_USART_C1,
	XA4U_USART_D0,
	XA4U_USART_D1,
	XA4U_USART_E0
} XA4U_USART_t;

// Error enum definition - used for error reporting
typedef enum XA4U_USART_ERR_enum
{
	XA4U_USART_ERR_NONE,	/* No error */
	XA4U_USART_ERR_FULL,	/* Buffer full; data not passed to buffer */
	XA4U_USART_ERR_EMTY,	/* Buffer empty; no data retrieved from buffer */
	XA4U_USART_ERR_HWNRDY,	/* TX hardware not ready; data not removed from buffer or passed to hardware buffer */
	XA4U_USART_ERR_UEFULL,	/* CRITICAL ERROR! Buffer filled unexpectedly after passing a check to prevent this */
	XA4U_USART_ERR_UEEMPTY,	/* CRITICAL ERROR! Buffer unexpectedly empty after passing a check to prevent this */
	XA4U_USART_ERR_INVD		/* CRITICAL ERROR! Invalid value passed as XA4U_USART_t; did you pass an int by mistake? */
} XA4U_USART_ERR_t;

/*  Adds an array of data to the specified USART's output buffer.
 *  INPUTS:
 *		'usart'     - USART buffer the data should be added to
 *		'data[]'    - Data to be added to the buffer
 *		'data_size' - Length of the array passed to 'data[]'
 *  RETURN:
 *		XA4U_USART_ERR_t - Error reporting enum; see above for definitions
 */
XA4U_USART_ERR_t USART_output_buffer_add(XA4U_USART_t usart, uint8_t data[], uint8_t data_size);


/*	Adds a byte of data to the USART input buffer. Meant to be called during USART receive ISR.
 *  INPUTS:
 *		'usart' - USART buffer the data should be added to
 *		'data'  - Data to be added to the buffer
 *  RETURN:
 *		XA4U_USART_ERR_t - Error reporting enum; see above for definitions
 */
XA4U_USART_ERR_t USART_input_buffer_add(XA4U_USART_t usart, uint8_t data);


/*	Reads and removes a byte of data from the USART input buffer.
 *  INPUTS:
 *		'usart' - USART buffer the data should be read from.
 *		'data'  - Data read from the buffer
 *  RETURN:
 *		XA4U_USART_ERR_t - Error reporting enum; see above for definitions
 */
XA4U_USART_ERR_t USART_read_byte(XA4U_USART_t usart, uint8_t *data);


/*	Sends a byte of data from a USART software output buffer to a USART hardware output buffer. Meant to be called during USART send ISR.
 *  INPUTS:
 *		'usart' - USART buffer to shift data byte from software to hardware
 *  RETURN:
 *		XA4U_USART_ERR_t - Error reporting enum; see above for definitions
 */
XA4U_USART_ERR_t USART_send_byte(XA4U_USART_t usart);


#endif
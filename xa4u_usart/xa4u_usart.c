/*
 * Name    : xa4u_usart.c
 * Created : 2/6/2019 1:26:47 AM
 * Author  : Andrew Bennett
 * 
 */ 

#include "xa4u_usart.h"

// Define USART data buffers
static volatile FIFOBuffer_t C0_out_buffer;
static volatile FIFOBuffer_t C1_out_buffer;
static volatile FIFOBuffer_t D0_out_buffer;
static volatile FIFOBuffer_t D1_out_buffer;
static volatile FIFOBuffer_t E0_out_buffer;
static volatile FIFOBuffer_t C0_in_buffer;
static volatile FIFOBuffer_t C1_in_buffer;
static volatile FIFOBuffer_t D0_in_buffer;
static volatile FIFOBuffer_t D1_in_buffer;
static volatile FIFOBuffer_t E0_in_buffer;

// Internal function to write to FIFO buffer. Returns XA4U_USART_ERR_FULL if buffer is full.
static XA4U_USART_ERR_t FIFO_write(volatile FIFOBuffer_t *buffer, uint8_t byte)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // Ensure buffer is accessed atomically
	{
		if((*buffer).buffer_head + 1 == (*buffer).buffer_tail || ((*buffer).buffer_head == USART_BUFFER_SIZE - 1 && (*buffer).buffer_tail == 0)) return XA4U_USART_ERR_FULL; // No room in buffer
		(*buffer).buffer[(*buffer).buffer_head] = byte;
		if((*buffer).buffer_head == USART_BUFFER_SIZE - 1) (*buffer).buffer_head = 0; // Handle buffer wrap-around
		else (*buffer).buffer_head++;
		(*buffer).buffer_size++;
	}
	return XA4U_USART_ERR_NONE;
}

// Internal function to read from FIFO buffer. Returns XA4U_USART_ERR_EMTY if buffer is empty.
static XA4U_USART_ERR_t FIFO_read(volatile FIFOBuffer_t *buffer, uint8_t *byte)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // Ensure buffer is accessed atomically
	{
		if((*buffer).buffer_head == (*buffer).buffer_tail) return XA4U_USART_ERR_EMTY; // Buffer empty
		*byte = (*buffer).buffer[(*buffer).buffer_tail];
		if((*buffer).buffer_tail == USART_BUFFER_SIZE - 1) (*buffer).buffer_tail = 0; // Handle buffer wrap-around
		else (*buffer).buffer_tail++;
		(*buffer).buffer_size--;
	}
	return XA4U_USART_ERR_NONE;
}

XA4U_USART_ERR_t USART_output_buffer_add(XA4U_USART_t usart, uint8_t data[], uint8_t data_size)
{
	XA4U_USART_ERR_t error = XA4U_USART_ERR_NONE;
	switch(usart)
	{
		case XA4U_USART_C0:
			if(USART_BUFFER_SIZE - C0_out_buffer.buffer_size < data_size) return XA4U_USART_ERR_FULL; // Not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFO_write(&C0_out_buffer, data[i]);
				if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_UEFULL; // Stop filling buffer if full, return unexpected full error
			}
			if(USARTC0.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty; initiates interrupt-driven TX until TX buffer is empty again
			{
				USART_send_byte(XA4U_USART_C0);
			}
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_C1:
			if(USART_BUFFER_SIZE - C1_out_buffer.buffer_size < data_size) return XA4U_USART_ERR_FULL; // Not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFO_write(&C1_out_buffer, data[i]);
				if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_UEFULL; // Stop filling buffer if full, return unexpected full error
			}
			if(USARTC1.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty; initiates interrupt-driven TX until TX buffer is empty again
			{
				USART_send_byte(XA4U_USART_C1);
			}
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D0:
			if(USART_BUFFER_SIZE - D0_out_buffer.buffer_size < data_size) return XA4U_USART_ERR_FULL; // Not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFO_write(&D0_out_buffer, data[i]);
				if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_UEFULL; // Stop filling buffer if full, return unexpected full error
			}
			if(USARTD0.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty; initiates interrupt-driven TX until TX buffer is empty again
			{
				USART_send_byte(XA4U_USART_D0);
			}
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D1:
			if(USART_BUFFER_SIZE - D1_out_buffer.buffer_size < data_size) return XA4U_USART_ERR_FULL; // Not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFO_write(&D1_out_buffer, data[i]);
				if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_UEFULL; // Stop filling buffer if full, return unexpected full error
			}
			if(USARTD1.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty; initiates interrupt-driven TX until TX buffer is empty again
			{
				USART_send_byte(XA4U_USART_D1);
			}
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_E0:
			if(USART_BUFFER_SIZE - E0_out_buffer.buffer_size < data_size) return XA4U_USART_ERR_FULL; // Not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFO_write(&E0_out_buffer, data[i]);
				if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_UEFULL; // Stop filling buffer if full, return unexpected full error
			}
			if(USARTE0.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty; initiates interrupt-driven TX until TX buffer is empty again
			{
				USART_send_byte(XA4U_USART_E0);
			}
			return XA4U_USART_ERR_NONE;
	}
	return XA4U_USART_ERR_INVD; // Return invalid XA4U_USART_t error if execution somehow reaches this point
}

XA4U_USART_ERR_t USART_input_buffer_add(XA4U_USART_t usart, uint8_t data)
{
	XA4U_USART_ERR_t error = XA4U_USART_ERR_NONE;
	switch(usart)
	{
		case XA4U_USART_C0:
			error = FIFO_write(&C0_in_buffer, data);
			if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_FULL; // Buffer full
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_C1:
			error = FIFO_write(&C1_in_buffer, data);
			if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_FULL; // Buffer full
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D0:
			error = FIFO_write(&D0_in_buffer, data);
			if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_FULL; // Buffer full
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D1:
			error = FIFO_write(&D1_in_buffer, data);
			if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_FULL; // Buffer full
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_E0:
			error = FIFO_write(&E0_in_buffer, data);
			if(error == XA4U_USART_ERR_FULL) return XA4U_USART_ERR_FULL; // Buffer full
			return XA4U_USART_ERR_NONE;
	}
	return XA4U_USART_ERR_INVD; // Return invalid XA4U_USART_t error if execution somehow reaches this point
}

XA4U_USART_ERR_t USART_read_byte(XA4U_USART_t usart, uint8_t *data)
{
	XA4U_USART_ERR_t error = XA4U_USART_ERR_NONE;
	uint8_t byte = 0x00;
	switch(usart)
	{
		case XA4U_USART_C0:
			error = FIFO_read(&C0_in_buffer, &byte);
			if(error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer empty
			*data = byte;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_C1:
			error = FIFO_read(&C1_in_buffer, &byte);
			if(error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer empty
			*data = byte;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D0:
			error = FIFO_read(&D0_in_buffer, &byte);
			if(error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer empty
			*data = byte;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D1:
			error = FIFO_read(&D1_in_buffer, &byte);
			if(error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer empty
			*data = byte;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_E0:
			error = FIFO_read(&E0_in_buffer, &byte);
			if(error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer empty
			*data = byte;
			return XA4U_USART_ERR_NONE;
	}
	return XA4U_USART_ERR_INVD; // Return invalid XA4U_USART_t error if execution somehow reaches this point
}

XA4U_USART_ERR_t USART_send_byte(XA4U_USART_t usart)
{
	XA4U_USART_ERR_t error = XA4U_USART_ERR_NONE;
	uint8_t data = 0x00;
	switch(usart)
	{
		case XA4U_USART_C0:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return XA4U_USART_ERR_HWNRDY;  // USART hardware is not ready
			error = FIFO_read(&C0_out_buffer, &data);
			if (error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer is empty
			USARTC0.DATA = data;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_C1:
			if (!(USARTC1.STATUS & USART_DREIF_bm)) return XA4U_USART_ERR_HWNRDY;  // USART hardware is not ready
			error = FIFO_read(&C1_out_buffer, &data);
			if (error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer is empty
			USARTC1.DATA = data;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D0:
			if (!(USARTD0.STATUS & USART_DREIF_bm)) return XA4U_USART_ERR_HWNRDY;  // USART hardware is not ready
			error = FIFO_read(&D0_out_buffer, &data);
			if (error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer is empty
			USARTD0.DATA = data;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_D1:
			if (!(USARTD1.STATUS & USART_DREIF_bm)) return XA4U_USART_ERR_HWNRDY;  // USART hardware is not ready
			error = FIFO_read(&D1_out_buffer, &data);
			if (error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer is empty
			USARTD1.DATA = data;
			return XA4U_USART_ERR_NONE;

		case XA4U_USART_E0:
			if (!(USARTE0.STATUS & USART_DREIF_bm)) return XA4U_USART_ERR_HWNRDY;  // USART hardware is not ready
			error = FIFO_read(&E0_out_buffer, &data);
			if (error == XA4U_USART_ERR_EMTY) return XA4U_USART_ERR_EMTY; // Buffer is empty
			USARTE0.DATA = data;
			return XA4U_USART_ERR_NONE;
	}
	return XA4U_USART_ERR_INVD; // Return invalid XA4U_USART_t error if execution somehow reaches this point
}
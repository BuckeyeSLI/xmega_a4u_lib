/*
 * Name    : xa4u_usart.h
 * Created : 2/12/2019 5:31:33 PM
 * Author  : Andrew Bennett
 * 
 * Description : Functions for setting up the clock source. These are for use at initialization only, they are not guaranteed to work if the OSC or CLK registers are
 *				 modified by other libraries.
 *               Note that F_CPU must still be defined in main.c (e.g. "define F_CPU 8000000UL" for an 8 MHz clock).
 */

#ifndef XA4U_CLOCK_H
#define XA4U_CLOCK_H

#include <avr/io.h>
#include <avr/delay.h>

// Clock enum definition - used to select which clock configuration is used
typedef enum XA4U_CLOCK_enum
{

	XA4U_32M,				/* 32 MHz internal RC clock */
	XA4U_32M_DFLL			/* 32MHz internal RC clock with DFLL */
} XA4U_CLOCK_t;


int clock_setup(XA4U_CLOCK_t mode);

#endif
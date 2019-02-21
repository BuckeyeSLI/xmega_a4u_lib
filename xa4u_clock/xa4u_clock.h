/*
 * Name    : xa4u_usart.h
 * Created : 2/12/2019 5:31:33 PM
 * Author  : Andrew Bennett
 * 
 * Description : Functions for setting up the clock source. These are for use at initialization only, they are not guaranteed to work if the OSC or CLK registers are
 *				 modified by other libraries.
 *               Note that F_CPU must still be defined in main.c (e.g. "define F_CPU 8000000UL" for an 8 MHz clock). It must be defined in before includes.
 */

#ifndef XA4U_CLOCK_H
#define XA4U_CLOCK_H

#include <avr/io.h>
#include <avr/cpufunc.h>

// Clock enum definition - used to select which clock configuration is used
typedef enum CLK_CFG_enum
{
	CLK_CFG_2M,			/* 2 MHz internal RC clock */
	CLK_CFG_2M_DFLL,	/* 2 MHz internal RC clock with DFLL */
	CLK_CFG_32K,		/* 32.768 kHz internal calibrated clock*/
	CLK_CFG_32M,		/* 32 MHz internal RC clock */
	CLK_CFG_32M_DFLL,	/* 32MHz internal RC clock with DFLL */
	CLK_CFG_EXT,		/* External crystal oscillator */
	CLK_CFG_EXT_32K,	/* External 32.768 kHz oscillator */
	CLK_CFG_EXTSRC		/* External clock source */
} CLK_CFG_t;

int clock_setup(CLK_CFG_t config);
/*	Initializes and sets the main CPU clock to the specified source.
 *  INPUTS:
 *		'config' - Clock source to use.
 */

#endif
/*
 * Name    : xa4u_usart.h
 * Created : 2/12/2019 5:31:33 PM
 * Author  : Andrew Bennett
 * 
 * Description : 
 */

 #ifndef XA4U_CLOCK_H
 #define XA4U_CLOCK_H

 #include <avr/io.h>

 // clock enum definition - used to select which clock configuration is used
 typedef enum XA4U_CLOCK_enum
 {
	 XA4U_32M,				/* 32 MHz internal RC clock */
	 XA4U_32M_DFLL			/* 32MHz internal RC clock with DFLL */
 } XA4U_CLOCK_t;
 
 #endif
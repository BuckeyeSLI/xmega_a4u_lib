/*
 * Name    : xa4u_clock.c
 * Created : 2/12/2019 5:31:33 PM
 * Author  : Andrew Bennett
 *
 */ 

#include "xa4u_clock.h"


void clock_config(CLK_CFG_t config)
{
	switch(config)
	{
		case CLK_CFG_2M:
			// Do nothing, this is the default configuration
			break;

		case CLK_CFG_2M_DFLL:
			// Enable 32 kHz clock
			OSC.CTRL |= OSC_RC32KEN_bm;
			// Wait for clock to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_RC32KRDY_bm));
			// Set DFLL frequency ratio to nominal for a 2 MHz clock
			DFLLRC2M.COMP1 = 0x07;
			DFLLRC2M.COMP2 = 0xA1;
			// Enable 2 MHz clock DFLL
			DFLLRC2M.CTRL = DFLL_ENABLE_bm;
			break;

		case CLK_CFG_32K:
			// Enable 32 kHz clock
			OSC.CTRL |= OSC_RC32KEN_bm;
			// Wait for clock to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_RC32KRDY_bm));
			// Set CPU clock to 32 kHz clock
			ccp_write_io((uint8_t*) &CLK.CTRL, CLK_SCLKSEL_RC32K_gc);
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		case CLK_CFG_32M:
			// Enable 32 MHz clock
			OSC.CTRL |= OSC_RC32MEN_bm;
			// Wait for clock to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_RC32MRDY_bm));
			// Set CPU clock to 32 MHz clock
			ccp_write_io((uint8_t*) &CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		case CLK_CFG_32M_DFLL:
			// Enable 32 MHz clock and 32 kHz clock
			OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm;
			// Wait for clocks to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_RC32MRDY_bm | OSC_RC32KRDY_bm));
			// Set CPU clock to 32 MHz clock
			ccp_write_io((uint8_t*) &CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
			// Set DFLL frequency ratio to nominal for a 32 MHz clock
			DFLLRC32M.COMP1 = 0x7A;
			DFLLRC32M.COMP2 = 0x12;
			// Enable 32 MHz clock DFLL
			DFLLRC32M.CTRL = DFLL_ENABLE_bm;
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		case CLK_CFG_EXT:
			// Set external crystal frequency range
			if(F_CPU >= 12000000UL) OSC.XOSCCTRL |= OSC_FRQRANGE_12TO16_gc;
			else if(F_CPU >= 9000000UL) OSC.XOSCCTRL |= OSC_FRQRANGE_9TO12_gc;
			else if(F_CPU >= 2000000UL) OSC.XOSCCTRL |= OSC_FRQRANGE_2TO9_gc;
			else OSC.XOSCCTRL |= OSC_FRQRANGE_04TO2_gc;
			// Set start-up time for maximum stability
			OSC.XOSCCTRL |= OSC_XOSCSEL_XTAL_16KCLK_gc;
			// Enable external crystal drive circuit
			OSC.CTRL |= OSC_XOSCEN_bm;
			// Wait for external crystal to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_XOSCRDY_bm));
			// Set CPU clock to external oscillator
			ccp_write_io((uint8_t*) &CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		case CLK_CFG_EXT_32K:
			// Select external 32kHz crystal
			OSC.XOSCCTRL |= OSC_XOSCSEL_32KHz_gc;
			// Enable external crystal drive circuit
			OSC.CTRL |= OSC_XOSCEN_bm;
			// Wait for external crystal to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_XOSCRDY_bm));
			// Set CPU clock to external oscillator
			ccp_write_io((uint8_t*) &CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		case CLK_CFG_EXTSRC:
			// Enable external clock input
			OSC.CTRL |= OSC_XOSCEN_bm;
			// Wait for external clock signal to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_XOSCRDY_bm));
			// Set CPU clock to external clock
			ccp_write_io((uint8_t*) &CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		default:
			break;
	}
}
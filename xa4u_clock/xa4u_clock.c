/*
 * Name    : xa4u_clock.c
 * Created : 2/12/2019 5:31:33 PM
 * Author  : Andrew Bennett
 *
 */ 

#include "xa4u_clock.h"

int clock_setup(XA4U_CLOCK_t mode)
{
	switch(mode)
	{
		case XA4U_32M:
			// Enable 32 MHz clock
			OSC.CTRL |= OSC_RC32MEN_bm;
			// Wait for clock to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_RC32MRDY_bm));
			// Set CPU clock to 32 MHz clock
			CCP = CCP_IOREG_gc;
			CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		case XA4U_32M_DFLL:
			// Enable 32 MHz clock and 32 kHz clock
			OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm;
			// Wait for clocks to stabilize
			while(OSC.STATUS != (OSC.STATUS | OSC_RC32MRDY_bm | OSC_RC32KRDY_bm));
			// Set CPU clock to 32 MHz clock
			CCP = CCP_IOREG_gc;
			CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
			// Set DFLL frequency ratio to nominal for a 32 MHz clock
			DFLLRC32M.COMP1 = 0x7A;
			DFLLRC32M.COMP2 = 0x12;
			// Enable 32 MHz clock DFLL
			DFLLRC32M.CTRL = DFLL_ENABLE_bm;
			// Disable 2 MHz clock
			OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;
			break;

		default:
			break;
	}
	return 0;
}
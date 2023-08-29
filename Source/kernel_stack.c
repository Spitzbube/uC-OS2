
#include "ucos_ii.h"
#include "kernel.h"

typedef void (*ISR)(void);
extern ISR intr_arFuncs[32];

int Data_234942a8 = 0; //234942a8 +0
void* OS_CPU_ExceptStkBase; //234942ac +4
int Data_234942b0; //234942b0 +8

extern void* OS_CPU_ExceptStk; //23487950


/* 23464598 - todo */
void OSInitHookBegin()
{
	uint32_t size;
	uint32_t* pstk;

	pstk = OS_CPU_ExceptStk;
	size = 0x1ff;
	while (size--)
	{
		*pstk++ = 0;
	}

	OS_CPU_ExceptStkBase = OS_CPU_ExceptStk;
	*(((uint32_t*)OS_CPU_ExceptStk) - 0x200) = 0x12345678;
}


/* 234645cc - complete */
void OSInitHookEnd()
{
#if 0
	((volatile uint32_t*)0xc2000000)[0] = '1';
#endif
}


/* 234645d0 - complete */
void OSTaskCreateHook()
{
#if 0
	((volatile uint32_t*)0xc2000000)[0] = '2';
#endif
}


/* 234645d4 - complete */
void OSTaskDelHook()
{
}


/* 234645d8 / 23477714? - todo */
void OSTaskIdleHook()
{
	if (Data_234942a8 != 0)
	{
		Data_234942a8--;
	}
	else
	{
		usb_RunBackground();

		Data_234942a8 = 100;
	}

	*((volatile uint32_t*)0xca00003c) = 0x1000; //FREG_CLKPWR_WATCHDOG
}


/* 23464610 - complete */
void sub_23464610()
{
#if 0
	((volatile uint32_t*)0xc2000000)[0] = '3';
#endif
}


/* 23464614 - todo */
uint32_t OSTaskStkInit(void (*func)(int), int b, uint32_t* stack, int d)
{
	*stack-- = (unsigned int)func & ~1;
	*stack-- = 0x14141414;
	*stack-- = 0x12121212;
	*stack-- = 0x11111111;
	*stack-- = 0x10101010;
	*stack-- = 0x09090909;
	*stack-- = 0x08080808;
	*stack-- = 0x07070707;
	*stack-- = 0x06060606;
	*stack-- = 0x05050505;
	*stack-- = 0x04040404;
	*stack-- = 0x03030303;
	*stack-- = 0x02020202;
	*stack-- = 0x01010101;
	*stack-- = b;
	*stack = ((uint32_t)func & 1)? 0x33: 0x13;

	return (uint32_t)stack;
}


/* 234646a0 - complete */
void OSTaskSwHook()
{
#if 0
	((volatile uint32_t*)0xc2000000)[0] = '4';
#endif
}


/* 234646a4 - complete */
void OSTimeTickHook()
{
#if 0
	((volatile uint32_t*)0xc2000000)[0] = '5';
#endif
}


/* 234646a8 - todo */
void OS_CPU_ExceptHndlr(uint32_t irqNr)
{
	uint32_t r5 = intr_get_vic_address();

#if 0
	((volatile uint32_t*)0xc2000000)[0] = 'i';

	switch (irqNr & 0x0f)
	{
	case 0:
		((volatile uint32_t*)0xc2000000)[0] = '0';
		break;
	case 1:
		((volatile uint32_t*)0xc2000000)[0] = '1';
		break;
	case 2:
		((volatile uint32_t*)0xc2000000)[0] = '2';
		break;
	case 3:
		((volatile uint32_t*)0xc2000000)[0] = '3';
		break;
	case 4:
		((volatile uint32_t*)0xc2000000)[0] = '4';
		break;
	case 5:
		((volatile uint32_t*)0xc2000000)[0] = '5';
		break;
	case 6:
		((volatile uint32_t*)0xc2000000)[0] = '6';
		break;
	case 7:
		((volatile uint32_t*)0xc2000000)[0] = '7';
		break;
	}
#endif

	if (((irqNr & 0x0f) == 6) && (((irqNr >> 8) & 0xff) != 0))
	{
#if 0
		switch ((irqNr >> 8) & 0xff)
		{
		case 1:
			((volatile uint32_t*)0xc2000000)[0] = 'a';
			break;
		case 2:
			((volatile uint32_t*)0xc2000000)[0] = 'b';
			break;
		case 3:
			((volatile uint32_t*)0xc2000000)[0] = 'c';
			break;
		case 4:
			((volatile uint32_t*)0xc2000000)[0] = 'd';
			break;
		case 5:
			((volatile uint32_t*)0xc2000000)[0] = 'e';
			break;
		case 6:
			((volatile uint32_t*)0xc2000000)[0] = 'f';
			break;
		case 7:
			((volatile uint32_t*)0xc2000000)[0] = 'g';
			break;
		case 8:
			((volatile uint32_t*)0xc2000000)[0] = 'h';
			break;
		case 9:
			((volatile uint32_t*)0xc2000000)[0] = 'i';
			break;
		case 10:
			((volatile uint32_t*)0xc2000000)[0] = 'j';
			break;
		case 11:
			((volatile uint32_t*)0xc2000000)[0] = 'k';
			break;
		case 12:
			((volatile uint32_t*)0xc2000000)[0] = 'l';
			break;
		case 13:
			((volatile uint32_t*)0xc2000000)[0] = 'm';
			break;
		case 14:
			((volatile uint32_t*)0xc2000000)[0] = 'n';
			break;
		case 15:
			((volatile uint32_t*)0xc2000000)[0] = 'o';
			break;
		case 16:
			((volatile uint32_t*)0xc2000000)[0] = 'p';
			break;
		case 17:
			((volatile uint32_t*)0xc2000000)[0] = 'q';
			break;
		case 18:
			((volatile uint32_t*)0xc2000000)[0] = 'r';
			break;
		case 19:
			((volatile uint32_t*)0xc2000000)[0] = 's';
			break;
		case 20:
			((volatile uint32_t*)0xc2000000)[0] = 't';
			break;
		case 21:
			((volatile uint32_t*)0xc2000000)[0] = 'u';
			break;
		case 22:
			((volatile uint32_t*)0xc2000000)[0] = 'v';
			break;
		case 23:
			((volatile uint32_t*)0xc2000000)[0] = 'w';
			break;
		case 24:
			((volatile uint32_t*)0xc2000000)[0] = 'x';
			break;
		case 25:
			((volatile uint32_t*)0xc2000000)[0] = 'y';
			break;
		case 26:
			((volatile uint32_t*)0xc2000000)[0] = 'z';
			break;
		}
#endif
		{
		void (*pFunc)(void) = intr_arFuncs[(irqNr >> 8) & 0xff];

		OS_CPU_SR_INT_En();

		if (pFunc) pFunc();
		}

		OS_CPU_SR_INT_Dis();
	}
	//loc_234646ec
	intr_set_vic_address(r5);
}



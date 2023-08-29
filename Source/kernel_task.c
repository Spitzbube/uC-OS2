
#include "ucos_ii.h"
#include "kernel.h"


/* 23439064 - todo */
char OSTaskCreateExt(void (*func)(int), int b,
    int* stack, unsigned int prio/*r3/r4*/,
    int e,
    int* f,
    int g/*r6*/,
    char* h,
    unsigned short i/*r7*/)
{
	char res;
	uint32_t cpu_status;
	int stack_frame;

	if (prio > 63)
	{
		//->loc_23448a54: OS_PRIO_INVALID
		res = 42;
	}
	else
	{
		cpu_status = FAMOS_EnterCriticalSection();

		if (rtos_arThread[prio] == 0)
		{
			rtos_arThread[prio] = (RTOS_tTCB*) 1;
			FAMOS_LeaveCriticalSection(cpu_status);

			if ((i & 1) || (i & 2))
			{
				memset(f, 0, g * 4);
			}

			stack_frame = OSTaskStkInit(func, b, stack, i);
			res = rtos_create_tcb(prio, stack_frame, f, e, g, h, i);

			if (res == 0)
			{
				cpu_status = FAMOS_EnterCriticalSection();
				OSTaskCtr++;
				FAMOS_LeaveCriticalSection(cpu_status);

				if (OSRunning == 1)
				{
					OS_Sched();
				}
				//->loc_23439138
			}
			else
			{
				//loc_23439128: TCB creation failed
				cpu_status = FAMOS_EnterCriticalSection();
				rtos_arThread[prio] = 0;
				FAMOS_LeaveCriticalSection(cpu_status);
			}
		}
		else
		{
			//loc_23439140: OS_PRIO_EXIST
			FAMOS_LeaveCriticalSection(cpu_status);

			res = 40;
		}
	}
	//loc_2343913c
	return res;
}


/* 2343914c - todo */
int OSTaskDel(uint8_t prio)
{
	uint32_t cpu_status;
	RTOS_tTCB* ptcb;

	if (OSIntNesting != 0)
	{
		return 63;
	}

	if (prio == 0x3f) //OS_TASK_IDLE_PRIO
	{
		return 61;
	}

	if (prio >= 0x3f)
	{
		if (prio != 0xff) //OS_PRIO_SELF
		{
			return 42; //OS_ERR_PRIO_INVALID
		}
	}
	//loc_23439184
	cpu_status = FAMOS_EnterCriticalSection();

	if (prio == 0xff)
	{
		prio = OSTCBCur->prio;
	}

	ptcb = rtos_arThread[prio];
	if (ptcb == 0)
	{
		//loc_234392b4
		FAMOS_LeaveCriticalSection(cpu_status);

		return 60;
	}

//	OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX;
	if ((OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0)
	{
		OSRdyGrp &= ~ptcb->OSTCBBitY;
	}

	if (ptcb->OSTCBEventPtr != 0)
	{
		OS_EVENT *pevent = ptcb->OSTCBEventPtr;
		uint8_t y =  ptcb->OSTCBY;

//		pevent->OSEventTbl[y]  &= ~ptcb->OSTCBBitX;
		if ((pevent->OSEventTbl[y]  &= ~ptcb->OSTCBBitX) == 0)
		{
			pevent->OSEventGrp &= ~ptcb->OSTCBBitY;
		}
	}
	//loc_23439208
	ptcb->OSTCBDly = 0;
	ptcb->OSTCBStat = 0;

    if (OSLockNesting < 255u)
    {
    	OSLockNesting++;
    }

	FAMOS_LeaveCriticalSection(cpu_status);

	OS_Dummy();

	cpu_status = FAMOS_EnterCriticalSection();

    if (OSLockNesting > 0)
    {
    	OSLockNesting--;
    }

    OSTaskDelHook(ptcb);

    OSTaskCtr--;

    rtos_arThread[prio] = 0;

    if (ptcb->OSTCBPrev == 0)
    {
    	ptcb->next->OSTCBPrev = 0;
    	OSTCBList = ptcb->next;
    }
    else
    {
    	ptcb->OSTCBPrev->next = ptcb->next;
    	ptcb->next->OSTCBPrev = ptcb->OSTCBPrev;
    }

    ptcb->next = rtos_pTCBFree;
    rtos_pTCBFree = ptcb;

	FAMOS_LeaveCriticalSection(cpu_status);

	OS_Sched();

	return 0;
}


/* 234392c0 - todo */
uint8_t OSTaskDelReq(uint8_t prio)
{
	uint32_t cpu_status;
	RTOS_tTCB* ptcb;
	uint8_t stat;

	if (prio == 0x3f) //OS_TASK_IDLE_PRIO
	{
		return 61;
	}

	if (prio >= 0x3f)
	{
		if (prio != 0xff) //OS_PRIO_SELF
		{
			return 42; //OS_ERR_PRIO_INVALID
		}

		cpu_status = FAMOS_EnterCriticalSection();

		stat = OSTCBCur->l;
		//->loc_23439318
		FAMOS_LeaveCriticalSection(cpu_status);

		return stat;
	}
	//loc_234392f8
	cpu_status = FAMOS_EnterCriticalSection();

	ptcb = /*OSTCBPrioTbl*/rtos_arThread[prio];
	if (ptcb != 0)
	{
		ptcb->l = 62;
		stat = 0;
	}
	else
	{
		stat = 11;
	}
	//loc_23439318
	FAMOS_LeaveCriticalSection(cpu_status);

	return stat;
}


/* 234395d0 - todo */
void rtos_task_wait(uint16_t a)
{
	if (a != 0)
	{
		uint32_t cpu_status;

		cpu_status = FAMOS_EnterCriticalSection();

		if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0)
		{
			OSRdyGrp &= ~OSTCBCur->OSTCBBitY;
		}

		OSTCBCur->OSTCBDly = a;

		FAMOS_LeaveCriticalSection(cpu_status);

		OS_Sched();
	}
}


/* 234396d8 - todo */
int OSTimeDlyResume(uint8_t prio)
{
	uint32_t r0;
	RTOS_tTCB* ptcb;

#if 0
	console_send_string("OSTimeDlyResume (todo.c): TODO\r\n");
#endif

	if (prio >= 0x3f)
	{
		return 42; //OS_PRIO_INVALID
	}

	r0 = FAMOS_EnterCriticalSection();

	ptcb = rtos_arThread[prio];

	if (ptcb == 0)
	{
		//loc_23439770
		FAMOS_LeaveCriticalSection(r0);
		return 11; //OS_TASK_NOT_EXIST
	}
	//0x23439700
	if (ptcb->OSTCBDly == 0)
	{
		//loc_23439764
		FAMOS_LeaveCriticalSection(r0);
		return 80; //OS_TIME_NOT_DLY
	}
	//0x2343970c
	ptcb->OSTCBDly = 0;

	if ((ptcb->OSTCBStat & 0x08/*OS_STAT_SUSPEND*/) == 0)
	{
		OSRdyGrp |= ptcb->OSTCBBitY;
		OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;

		FAMOS_LeaveCriticalSection(r0);

		OS_Sched();
	}
	else
	{
		//loc_23439758
		FAMOS_LeaveCriticalSection(r0);
	}
	//loc_2343975c
	return 0;
}




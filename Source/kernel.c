
#include "ucos_ii.h"
#include "kernel.h"

const uint8_t Data_23489cc0[] = //23489cc0
{
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

const uint8_t OSUnMapTbl[] = //23489cc8
{
0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x07, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00
};


uint8_t OSIntNesting; //23492c00 +0
uint8_t bData_23492c01; //23492c01 +1
uint8_t OSLockNesting; //23492c02 +2 //3a863a
uint8_t OSPrioCur; //23492c03 +3
uint8_t OSPrioHighRdy; //23492c04 +4
uint8_t OSRdyGrp; //23492c05 +5 //bData_3a863d
uint8_t OSRunning; //23492c06 +6
uint8_t OSTaskCtr; //23492c07 +7 //bData_3a8647
int OSCtxSwCtr; //23492c08 +8 //Data_3a7f74
OS_EVENT* OSEventFreeList; //23492c0c +0xc
int OSIdleCtr; //23492c10 +0x10
RTOS_tTCB* OSTCBCur; //23492c14 +0x14
RTOS_tTCB* rtos_pTCBFree; //23492c18 +0x18
RTOS_tTCB* OSTCBHighRdy; //23492c1c +0x1c
RTOS_tTCB* OSTCBList; //23492c20 +0x20
int OSTime; //23492c24 +0x24
uint8_t OSRdyTbl[8]; //23492c28 +0x28
OS_Q* OSQFreeList; //23492c34 +0x34

OS_EVENT OSEventTbl[200]; //235f7b3c +200*20 = 235F8ADC
int OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE]; //235f8adc +0x400*4 = 235F9AD8+4
//int Data_237814d8; //237814d8
RTOS_tTCB* rtos_arThread[64]; //235f9adc +64*4 = 235F9BDC
RTOS_tTCB rtos_arTCB[32]; //235f9bdc +32*0x30 = 235FA1DC
OS_Q OSQTbl[OS_MAX_QS]; //235fa400


static void rtos_thread_init(void)
{
	uint8_t k;

	for (k = 0; k < 64; k++)
	{
		//loc_2343880c
		rtos_arThread[k] = 0;
	}

	{
		RTOS_tTCB* current = &rtos_arTCB[0];
		RTOS_tTCB* first = current;
		RTOS_tTCB* next = current + 1; //lr + 0x30;

		for (k = 0; k < 32; k++, current++, next++)
		{
			current->next = next;
		}

		current->next = 0;

		rtos_pTCBFree = first; //+0x18
	}
}


static void OS_InitEventList(void)
{
  unsigned short i;

  OS_EVENT* current = OSEventTbl;
  OS_EVENT* next = current + 1;

  for (i = 0; i < 199; i++, current++, next++)
  {
    current->OSEventType = 0;
    current->OSEventPtr = next;
  }

  current->OSEventType = 0;
  current->OSEventPtr = 0;

  OSEventFreeList = OSEventTbl;
}


/* 2343877C - complete */
void OS_TaskIdle()
{
	while (1)
	{
		uint32_t cpu_status = FAMOS_EnterCriticalSection();

#if 0
		console_send_string("OS_TaskIdle\r\n");
#endif

		OSIdleCtr++;

		FAMOS_LeaveCriticalSection(cpu_status);

		OSTaskIdleHook();
	}
}


static  void  OS_InitRdyList (void)
{
    INT16U  i;


    OSRdyGrp      = 0u;                                    /* Clear the ready list                     */

    INT8U* r1 = &OSRdyTbl[0];

    for (i = 0u; i < 8/*OS_RDY_TBL_SIZE*/; i++) {
#if 0
        OSRdyTbl[i] = 0u;
#else
        *r1++ = 0u;
#endif
    }

    OSPrioCur     = 0u;
    OSPrioHighRdy = 0u;

    OSTCBHighRdy  = /*(OS_TCB *)*/0;
    OSTCBCur      = /*(OS_TCB *)*/0;
}


/* 234480b8 - todo */
void OSInit()
{
	OSInitHookBegin();

	OSTime = 0; //+0x24
	OSIntNesting = 0; //+0
	OSLockNesting = 0; //+2
	OSTaskCtr = 0; //+7
	OSRunning = 0; //+6
	OSCtxSwCtr = 0; //+8
	OSIdleCtr = 0; //+0x10

	OS_InitRdyList();

	OSTCBList = 0; //+0x20

	rtos_thread_init();
	OS_InitEventList();
	OS_MemInit();
	OS_QInit();

	OSTaskCreateExt(OS_TaskIdle,
			0,
			&OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE-1],
			OS_TASK_IDLE_PRIO,
			OS_TASK_IDLE_ID,
			OSTaskIdleStk,
			OS_TASK_IDLE_STK_SIZE,
			0,
			0x03);

	OSInitHookEnd();
}


/* 234388f8 - todo */
void OSIntExit()
{
	if (OSRunning == 1)
	{
		uint32_t cpu_status = FAMOS_EnterCriticalSection();

		if (OSIntNesting != 0)
		{
			OSIntNesting--;
		}

		if ((OSIntNesting == 0) && (OSLockNesting == 0))
		{
			bData_23492c01 = OSUnMapTbl[OSRdyGrp];

		    OSPrioHighRdy = OSUnMapTbl[OSRdyTbl[bData_23492c01]] + (bData_23492c01 * 8);

		    if (OSPrioHighRdy != OSPrioCur)
		    {
		    	OSTCBHighRdy = rtos_arThread[OSPrioHighRdy];

		    	OSCtxSwCtr++;

		    	OSIntCtxSw();
		    }
		}
		//loc_23438984
		FAMOS_LeaveCriticalSection(cpu_status);
	}
}


/* 234389c0 (1c8d10) - todo */
void OS_Sched()
{
	uint32_t cpu_status = FAMOS_EnterCriticalSection();

	if ((OSIntNesting == 0) && (OSLockNesting == 0))
	{
		int r0 = OSUnMapTbl[OSRdyGrp];

		OSPrioHighRdy = (r0 * 8) + OSUnMapTbl[OSRdyTbl[r0]];

		if (OSPrioHighRdy != OSPrioCur)
		{
			OSTCBHighRdy = rtos_arThread[OSPrioHighRdy];
			OSCtxSwCtr++;
			OSCtxSw();
		}
	}

	FAMOS_LeaveCriticalSection(cpu_status);
}


/* 23438a88 - todo */
void rtos_start()
{
	if (OSRunning == 0)
	{
	    int r1 = OSUnMapTbl[OSRdyGrp];

	    OSPrioHighRdy = OSUnMapTbl[OSRdyTbl[r1]] + (r1 * 8);
	    OSPrioCur = OSPrioHighRdy;

	    OSTCBHighRdy = rtos_arThread[OSPrioHighRdy];
	    OSTCBCur = OSTCBHighRdy;

	    OSStartHighRdy();
	}
}


/* 23438ad4 - todo */
void OSTimeTick()
{
	RTOS_tTCB* pTask;
	uint32_t r0;

	OSTimeTickHook();
	r0 = FAMOS_EnterCriticalSection();

	OSTime++;

	FAMOS_LeaveCriticalSection(r0);

	if (OSRunning == 1)
	{
		//->loc_23438b70
		for (pTask = OSTCBList; pTask->prio != 63; )
		{
			//loc_23438b10
			r0 = FAMOS_EnterCriticalSection();

			if (pTask->OSTCBDly != 0)
			{
				pTask->OSTCBDly--;
				if (pTask->OSTCBDly == 0)
				{
					if ((pTask->OSTCBStat & 8) != 0)
					{
						pTask->OSTCBDly = 1;
					}
					else
					{
						OSRdyGrp                |= pTask->OSTCBBitY;
						OSRdyTbl[pTask->OSTCBY] |= pTask->OSTCBBitX;
					}
				}

			}
			//loc_23438b68
			pTask = pTask->next;

			FAMOS_LeaveCriticalSection(r0);
		}
	}
}


/* 23438b88 - complete */
void OS_Dummy()
{
}


/* 23438b8c - todo */
int OS_EventTaskRdy(OS_EVENT *pevent, void *pmsg, int msk)
{
	uint8_t y;
	uint8_t x;
	int lr;
	int r4;
	uint8_t prio;
	RTOS_tTCB* ptcb;

	y = OSUnMapTbl[pevent->OSEventGrp];
	lr = Data_23489cc0[y];
	x = OSUnMapTbl[pevent->OSEventTbl[y]/*r6*/];
	r4 = Data_23489cc0[x];
	prio = (y << 3) + x;

	if ((pevent->OSEventTbl[y] &= ~r4) == 0)
	{
		pevent->OSEventGrp &= ~lr;
	}

	ptcb = rtos_arThread[prio];

	ptcb->OSTCBDly = 0;
	ptcb->OSTCBMsg = pmsg;
	ptcb->OSTCBEventPtr = 0;
	ptcb->OSTCBStat &= ~msk;
	if ((ptcb->OSTCBStat & 0xff) == 0)
	{
		OSRdyGrp |= lr;
		OSRdyTbl[y] |= r4;
	}
	//loc_23438c1c
	return prio;
}


/* 23438c24 - todo */
void OS_EventTaskWait(OS_EVENT* pevent)
{
    uint8_t  y;

    OSTCBCur->OSTCBEventPtr               = pevent;                 /* Store ptr to ECB in TCB         */

    y             =  OSTCBCur->OSTCBY;            /* Task no longer ready                              */
    if ((OSRdyTbl[y]  &= ~OSTCBCur->OSTCBBitX) == 0)
    {
        OSRdyGrp &= ~OSTCBCur->OSTCBBitY;         /* Clear event grp bit if this was only task pending */
    }

    pevent->OSEventTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;    /* Put task in waiting list        */
    pevent->OSEventGrp                   |= OSTCBCur->OSTCBBitY;

}


/* 23438c8c - todo */
void OS_EventTaskRemove(OS_EVENT *pevent)
{
    uint8_t  y;

    y                       =  OSTCBCur->OSTCBY;
//    pevent->OSEventTbl[y]  &= ~OSTCBCur->OSTCBBitX;         /* Remove task from wait list                  */
//    if (pevent->OSEventTbl[y] == 0)
    if ((pevent->OSEventTbl[y]  &= ~OSTCBCur->OSTCBBitX) == 0)
    {
        pevent->OSEventGrp &= ~OSTCBCur->OSTCBBitY;
    }

    OSTCBCur->OSTCBStat = 0;
    OSTCBCur->OSTCBEventPtr = 0;
}


/* 23438cd4 - todo */
void OS_EventWaitListInit(OS_EVENT *pevent)
{
	uint8_t* ptbl;
	uint8_t i;

    pevent->OSEventGrp = 0;                      /* No task waiting on event                           */
    ptbl               = &pevent->OSEventTbl[0];

    for (i = 0; i < 8/*OS_EVENT_TBL_SIZE*/; i++)
    {
        *ptbl++ = 0;
    }

}

#if 0
0x23438cd4 0010A0E3               mov        r1, #0x0                           ; CODE XREF=sub_2346370c+88, sub_23463cac+152, rtos_create_sema+88
0x23438cd8 0110C0E5               strb       r1, [r0, #0x1]
0x23438cdc 0C10C0E5               strb       r1, [r0, #0xc]
0x23438ce0 0D10C0E5               strb       r1, [r0, #0xd]
0x23438ce4 0E10C0E5               strb       r1, [r0, #0xe]
0x23438ce8 0F10C0E5               strb       r1, [r0, #0xf]
0x23438cec 1010C0E5               strb       r1, [r0, #0x10]
0x23438cf0 1110C0E5               strb       r1, [r0, #0x11]
0x23438cf4 1210C0E5               strb       r1, [r0, #0x12]
0x23438cf8 1310C0E5               strb       r1, [r0, #0x13]
												  #endif


/* 23438d00 - todo */
int rtos_create_tcb(int prio/*r5*/, int stack_frame/*r6*/, int* f/*r8*/,
                    int e, int g/*sb*/, char* name/*sl*/, int i/*fp*/)
{
	RTOS_tTCB* tcb;
	uint32_t cpu_status;

	cpu_status = FAMOS_EnterCriticalSection();

	tcb = rtos_pTCBFree;
	if (tcb != 0)
	{
	    rtos_pTCBFree = tcb->next;

		FAMOS_LeaveCriticalSection(cpu_status);

	    tcb->stack_frame = stack_frame;
	    tcb->prio = prio;
	    tcb->OSTCBStat = 0;
	    tcb->OSTCBDly = 0;
	    tcb->c = (int) f;
	    tcb->d = g;
	    tcb->name = name;
	    tcb->e1 = i;
	    tcb->e2 = e;
	    tcb->OSTCBY = prio >> 3;
	    tcb->OSTCBBitY = Data_23489cc0[tcb->OSTCBY];
	    tcb->l = 0;
	    tcb->k1 = prio & 0x07;
	    tcb->OSTCBBitX = Data_23489cc0[tcb->k1];
	    tcb->OSTCBEventPtr = 0;
	    tcb->OSTCBMsg = 0;

	    sub_23464610(tcb);
	    OSTaskCreateHook(tcb);

		cpu_status = FAMOS_EnterCriticalSection();

	    rtos_arThread[prio] = tcb;
	    tcb->next = OSTCBList;
	    tcb->OSTCBPrev = 0;
	    if (OSTCBList != 0)
	    {
	    	OSTCBList->OSTCBPrev = tcb;
	    }
	    OSTCBList = tcb;

	    OSRdyGrp |= tcb->OSTCBBitY;

	    OSRdyTbl[tcb->OSTCBY] |= tcb->OSTCBBitX;

		FAMOS_LeaveCriticalSection(cpu_status);

		return 0;
	}
	else
	{
		//loc_23448718: OS_NO_MORE_TCB
		FAMOS_LeaveCriticalSection(cpu_status);
		return 70;
	}
}



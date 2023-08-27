
#include <ucos_ii.h>
#include "kernel_sema.h"



/* 234641b8 - complete */
int OSSemAccept(OS_EVENT* pevent)
{
	uint32_t r0;
	uint16_t cnt;

#if 0
	console_send_string("OSSemAccept (todo.c): TODO\r\n");
#endif

	if (pevent == 0)
	{
		return 0;
	}

	if (pevent->OSEventType != 3)
	{
		return 0;
	}

	r0 = FAMOS_EnterCriticalSection();

	cnt = pevent->OSEventCnt;
	if (cnt != 0)
	{
		pevent->OSEventCnt--;
	}

	FAMOS_LeaveCriticalSection(r0);

	return cnt;
}


/* 234641f8 - todo */
OS_EVENT* OSSemCreate(INT16U cnt)
{
	OS_EVENT* pEvent;
	uint32_t cpu_status;

	if (OSIntNesting != 0)
	{
		return 0;
	}

	cpu_status = FAMOS_EnterCriticalSection();

	pEvent = OSEventFreeList;
	if (pEvent != 0)
	{
		OSEventFreeList = pEvent->OSEventPtr;
	}

	FAMOS_LeaveCriticalSection(cpu_status);

	if (pEvent != 0)
	{
		pEvent->OSEventType = 3; //OS_EVENT_TYPE_SEM
		pEvent->OSEventCnt = cnt;
		pEvent->OSEventPtr = 0;

		OS_EventWaitListInit(pEvent);
	}
	//loc_23464254
	return pEvent;
}


/* 2346425c - todo */
OS_EVENT* OSSemDel(OS_EVENT* pevent, int opt, int* res)
{
	uint32_t cpu_status;
	int r6;

	if (OSIntNesting != 0)
	{
		*res = 0x8c; //OS_ERR_DEL_ISR
		//->loc_234642fc
		return pevent;
	}

	if (pevent == 0)
	{
		*res = 4;
		//->loc_234642fc
		return pevent;
	}

	if (pevent->OSEventType != 3)
	{
		*res = 1;
		//->loc_234642fc
		return pevent;
	}

	cpu_status = FAMOS_EnterCriticalSection();

	if (pevent->OSEventGrp != 0)
	{
		r6 = 1;
	}
	else
	{
		r6 = 0;
	}

	switch (opt)
	{
	case 0:
		//loc_234642cc
		if (r6 == 0)
		{
			pevent->OSEventType = 0;
			pevent->OSEventPtr = OSEventFreeList;
			OSEventFreeList = pevent;

			FAMOS_LeaveCriticalSection(cpu_status);
			//->loc_23464344
			*res = 0;
			return 0;
		}
		else
		{
			//loc_234642f0
			FAMOS_LeaveCriticalSection(cpu_status);

			*res = 8;
			return pevent;
		}
		break;

	case 1:
		//loc_23464318
		while (pevent->OSEventGrp != 0)
		{
			//->loc_23464308
			OS_EventTaskRdy(pevent, 0, 1);
		}
		//0x23464324
		pevent->OSEventType = 0;
		pevent->OSEventPtr = OSEventFreeList;
		OSEventFreeList = pevent;

		FAMOS_LeaveCriticalSection(cpu_status);

		if (r6 == 1)
		{
			OS_Sched();
		}
		//->loc_23464344
		*res = 0;
		return 0;
		break;

	default:
		//loc_23464350
		FAMOS_LeaveCriticalSection(cpu_status);
		*res = 7; //OS_ERR_INVALID_OPT
		//->loc_234642fc
		return pevent;
		break;
	}
}


/* 23464360 - todo */
void OSSemPend(OS_EVENT* sema, INT16U r8, INT8U* res)
{
	uint32_t cpu_status;

	if (OSIntNesting != 0)
	{
		//->23477554
		*res = 2;
	}
	else if (sema == 0)
	{
		*res = 4;
	}
	else if (sema->OSEventType != 3)
	{
		*res = 1;
	}
	else
	{
		cpu_status = FAMOS_EnterCriticalSection();

		if (sema->OSEventCnt != 0)
		{
			sema->OSEventCnt--;
			//->loc_23477560
			FAMOS_LeaveCriticalSection(cpu_status);
			*res = 0;
		}
		else
		{
			OSTCBCur->OSTCBStat |= 0x01;
			OSTCBCur->OSTCBDly = r8;

			OS_EventTaskWait(sema);
			FAMOS_LeaveCriticalSection(cpu_status);

			OS_Sched();

			cpu_status = FAMOS_EnterCriticalSection();

			if (OSTCBCur->OSTCBStat & 0x01)
			{
				//23477540
				OS_EventTaskRemove(sema);
				FAMOS_LeaveCriticalSection(cpu_status);
				*res = 10; //OS_ERR_TIMEOUT
			}
			else
			{
				//loc_2347755c
				OSTCBCur->OSTCBEventPtr = 0;
				//loc_23477560
				FAMOS_LeaveCriticalSection(cpu_status);
				*res = 0;
			}
		}
	}
}


/* 23464434 - todo */
INT8U OSSemPost(OS_EVENT* pevent)
{
	uint32_t cpu_status;

	if (pevent == 0)
	{
		return 4;
	}

	if (pevent->OSEventType != 3)
	{
		return 1;
	}

	cpu_status = FAMOS_EnterCriticalSection();

	if (pevent->OSEventGrp != 0)
	{
		OS_EventTaskRdy(pevent, 0, 1);

		FAMOS_LeaveCriticalSection(cpu_status);

		OS_Sched();
		//->loc_234644a8
		return 0;
	}
	//loc_23464488
	if (pevent->OSEventCnt < 0xffff)
	{
		pevent->OSEventCnt++;

		FAMOS_LeaveCriticalSection(cpu_status);

		return 0;
	}
	//loc_234644b0
	FAMOS_LeaveCriticalSection(cpu_status);

	return 50; //OS_ERR_SEM_OVF
}


#if 0
/* 23477570 - todo */
void sub_23477570_()
{

}
#endif


int OSSemQuery(OS_EVENT* pevent, OS_SEM_DATA* pData)
{
	int i;
	uint32_t cpu_status;

	if (pevent == 0)
	{
		return 4;
	}

	if (pevent->OSEventType != 3)
	{
		return 1;
	}

	cpu_status = FAMOS_EnterCriticalSection();

	pData->bData_10 = pevent->OSEventGrp;

	for (i = 0; i < 8; i++)
	{
		pData->arData_2[i] = pevent->OSEventTbl[i];
	}

	pData->wData_0 = pevent->OSEventCnt;

	FAMOS_LeaveCriticalSection(cpu_status);

	return 0;
}



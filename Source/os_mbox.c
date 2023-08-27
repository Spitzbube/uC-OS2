

#include <ucos_ii.h>


/* 234636d0 - todo */
void  *OSMboxAccept (OS_EVENT *pevent)
{
    void      *pmsg;
	uint32_t cpu_status;

    if (pevent == 0)
    {
        return 0;
    }

    if (pevent->OSEventType != 1/*OS_EVENT_TYPE_MBOX*/)
    {
        return 0;
    }

	cpu_status = FAMOS_EnterCriticalSection();

    pmsg               = pevent->OSEventPtr;
    pevent->OSEventPtr = 0;

	FAMOS_LeaveCriticalSection(cpu_status);

	return (pmsg);
}


/* 2346370c - todo */
OS_EVENT* OSMboxCreate(void* pmsg)
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
		pEvent->OSEventType = 1; //OS_EVENT_TYPE_MBOX
		pEvent->OSEventCnt = 0;
		pEvent->OSEventPtr = pmsg;

		OS_EventWaitListInit(pEvent);
	}
	//loc_23463768
	return pEvent;
}


/* 23463874 - todo */
void* OSMboxPend(OS_EVENT* pevent, int timeout, uint8_t* perr)
{
	uint32_t cpu_status;
	void* pmsg;

	if (OSIntNesting != 0)
	{
		*perr = 2;
		return 0;
	}

	if (pevent == 0)
	{
		*perr = 4;
		return 0;
	}

	if (pevent->OSEventType != 1)
	{
		*perr = 1;
		return 0;
	}

	cpu_status = FAMOS_EnterCriticalSection();

	pmsg = pevent->OSEventPtr;
	if (pmsg != 0)
	{
		pevent->OSEventPtr = 0;
		//->loc_23463920
		FAMOS_LeaveCriticalSection(cpu_status);

		*perr = 0;
		return pmsg;
	}

	OSTCBCur->OSTCBStat     |= 2; //OS_STAT_MBOX;          /* Message not available, task will pend         */
//	    OSTCBCur->OSTCBStatPend  = OS_STAT_PEND_OK;
	OSTCBCur->OSTCBDly       = timeout;                /* Load timeout in TCB                           */

	OS_EventTaskWait(pevent);                          /* Suspend task until event or timeout occurs    */

	FAMOS_LeaveCriticalSection(cpu_status);

	OS_Sched();                                        /* Find next highest priority task ready to run  */

	cpu_status = FAMOS_EnterCriticalSection();

	pmsg = OSTCBCur->OSTCBMsg;

	if (pmsg != 0)
	{
		OSTCBCur->OSTCBMsg = 0;
		OSTCBCur->OSTCBStat = 0;
		OSTCBCur->OSTCBEventPtr = 0;

		FAMOS_LeaveCriticalSection(cpu_status);

		*perr = 0;
		return pmsg;
	}
	else
	{
		//loc_23463934
		OS_EventTaskRemove(pevent);
		FAMOS_LeaveCriticalSection(cpu_status);

		*perr = 10; //OS_ERR_TIMEOUT
		return 0;
	}

	return 0;
}


/* 23463954 - todo */
int OSMboxPost(OS_EVENT *pevent, void *pmsg)
{
	uint32_t cpu_status;

	if (pevent == 0)
	{
		return 4;
	}

	if (pmsg == 0)
	{
		return 3;
	}

	if (pevent->OSEventType != 1)
	{
		return 1;
	}

	cpu_status = FAMOS_EnterCriticalSection();

	if (pevent->OSEventGrp != 0)
	{
		OS_EventTaskRdy(pevent, pmsg, 2);

		FAMOS_LeaveCriticalSection(cpu_status);

		OS_Sched();
		//->loc_234639e0
	}
	else
	{
		//loc_234639b8
		if (pevent->OSEventPtr != 0)
		{
			FAMOS_LeaveCriticalSection(cpu_status);

			return 20; //OS_ERR_MBOX_FULL
		}
		//loc_234639d4
		pevent->OSEventPtr = pmsg;

		FAMOS_LeaveCriticalSection(cpu_status);
	}
	//loc_234639e0
	return 0;
}




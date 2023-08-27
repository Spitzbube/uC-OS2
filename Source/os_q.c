
#include "ucos_ii.h"

#if 0
extern OS_Q* OSQFreeList; //23492c34
extern OS_Q OSQTbl[];
#endif


/* 23463b0c - todo */
void sub_23463b0c()
{
	console_send_string("sub_23463b0c (todo.c): TODO\r\n");

}


/* 23463c3c - todo */
void* OSQAccept (OS_EVENT* pevent/*, uint8_t *err*/)
{
    void      *msg;
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif
    uint32_t cpu_sr;



#if OS_ARG_CHK_EN > 0
    if (err == (uint8_t *)0) {                     /* Validate 'err'                                     */
        return ((void *)0);
    }
#endif
    if (pevent == (OS_EVENT *)0) {               /* Validate 'pevent'                                  */
//        *err = OS_ERR_PEVENT_NULL;
        return ((void *)0);
    }
    if (pevent->OSEventType != 2/*OS_EVENT_TYPE_Q*/) {/* Validate event block type                          */
//        *err = 1; //OS_ERR_EVENT_TYPE;
        return ((void *)0);
    }
    cpu_sr = FAMOS_EnterCriticalSection();
    pq = (OS_Q *)pevent->OSEventPtr;             /* Point at queue control block                       */
    if (pq->OSQEntries > 0) {                    /* See if any messages in the queue                   */
        msg = *pq->OSQOut++;                     /* Yes, extract oldest message from the queue         */
        pq->OSQEntries--;                        /* Update the number of entries in the queue          */
        if (pq->OSQOut == pq->OSQEnd) {          /* Wrap OUT pointer if we are at the end of the queue */
            pq->OSQOut = pq->OSQStart;
        }
//        *err = 0; //OS_NO_ERR;
    }
    else {
//        *err = OS_Q_EMPTY;
        msg  = (void *)0;                        /* Queue is empty                                     */
    }
    FAMOS_LeaveCriticalSection(cpu_sr);
    return (msg);                                /* Return message received (or NULL)                  */
}


/* 23463cac - complete */
OS_EVENT* OSQCreate(void **start, uint16_t size)
{
	OS_EVENT  *pevent; //r5
    OS_Q      *pq; //r4
	uint32_t cpu_sr;

#if 0
	console_send_string("OSQCreate (todo.c): TODO\r\n");
#endif

	if (OSIntNesting != 0)
	{
		return 0;
	}

	cpu_sr = FAMOS_EnterCriticalSection();

	pevent = OSEventFreeList;

	if (OSEventFreeList != 0)
	{
		OSEventFreeList = (OS_EVENT *)OSEventFreeList->OSEventPtr;
	}

	FAMOS_LeaveCriticalSection(cpu_sr);

	if (pevent != 0)
	{
		cpu_sr = FAMOS_EnterCriticalSection();

		pq = OSQFreeList;

		if (pq != 0)
		{
			OSQFreeList = OSQFreeList->OSQPtr;

			FAMOS_LeaveCriticalSection(cpu_sr);

			pq->OSQStart           = start;
			pq->OSQEnd             = &start[size];
			pq->OSQIn              = start;
			pq->OSQOut             = start;
			pq->OSQSize            = size;
			pq->OSQEntries         = 0;

			pevent->OSEventType = 2; //OS_EVENT_TYPE_Q
			pevent->OSEventCnt = 0;
			pevent->OSEventPtr     = pq;

			OS_EventWaitListInit(pevent);
			//->loc_23463d60
		}
		else
		{
			//loc_23463d4c
			pevent->OSEventPtr = (void *)OSEventFreeList;
			OSEventFreeList    = pevent;

			FAMOS_LeaveCriticalSection(cpu_sr);

			pevent = 0;
		}
	}
	//loc_23463d60
	return pevent;
}


/* 23463d68 - todo */
OS_EVENT  *OSQDel (OS_EVENT *pevent, uint8_t opt, uint8_t *err)
{
#if 1
	console_send_string("OSQDel (todo.c): TODO\r\n");
#else
    BOOLEAN    tasks_waiting;
    OS_EVENT  *pevent_return;
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

    uint32_t cpu_sr;


#if OS_ARG_CHK_EN > 0
    if (err == (uint8_t *)0) {                               /* Validate 'err'                           */
        return (pevent);
    }
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return (pevent);
    }
#endif
    if (pevent->OSEventType != 2/*OS_EVENT_TYPE_Q*/) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);
    }
    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);
    }
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0) {                         /* See if any tasks waiting on queue        */
        tasks_waiting = OS_TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = OS_FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete queue only if no task waiting     */
             if (tasks_waiting == OS_FALSE) {
#if OS_EVENT_NAME_SIZE > 1
                 pevent->OSEventName[0] = '?';             /* Unknown name                             */
                 pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
                 pq                     = (OS_Q *)pevent->OSEventPtr;  /* Return OS_Q to free list     */
                 pq->OSQPtr             = OSQFreeList;
                 OSQFreeList            = pq;
                 pevent->OSEventType    = OS_EVENT_TYPE_UNUSED;
                 pevent->OSEventPtr     = OSEventFreeList; /* Return Event Control Block to free list  */
                 pevent->OSEventCnt     = 0;
                 OSEventFreeList        = pevent;          /* Get next free event control block        */
                 OS_EXIT_CRITICAL();
                 *err                   = OS_NO_ERR;
                 pevent_return          = (OS_EVENT *)0;   /* Queue has been deleted                   */
             } else {
                 OS_EXIT_CRITICAL();
                 *err                   = OS_ERR_TASK_WAITING;
                 pevent_return          = pevent;
             }
             break;

        case OS_DEL_ALWAYS:                                /* Always delete the queue                  */
             while (pevent->OSEventGrp != 0) {             /* Ready ALL tasks waiting for queue        */
                 (void)OS_EventTaskRdy(pevent, (void *)0, OS_STAT_Q);
             }
#if OS_EVENT_NAME_SIZE > 1
             pevent->OSEventName[0] = '?';                 /* Unknown name                             */
             pevent->OSEventName[1] = OS_ASCII_NUL;
#endif
             pq                     = (OS_Q *)pevent->OSEventPtr;   /* Return OS_Q to free list        */
             pq->OSQPtr             = OSQFreeList;
             OSQFreeList            = pq;
             pevent->OSEventType    = OS_EVENT_TYPE_UNUSED;
             pevent->OSEventPtr     = OSEventFreeList;     /* Return Event Control Block to free list  */
             pevent->OSEventCnt     = 0;
             OSEventFreeList        = pevent;              /* Get next free event control block        */
             FAMOS_LeaveCriticalSection(cpu_sr);
             if (tasks_waiting == OS_TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err                   = OS_NO_ERR;
             pevent_return          = (OS_EVENT *)0;       /* Queue has been deleted                   */
             break;

        default:
        	FAMOS_LeaveCriticalSection(cpu_sr);
             *err                   = OS_ERR_INVALID_OPT;
             pevent_return          = pevent;
             break;
    }
    return (pevent_return);
#endif
}


uint8_t  OSQFlush (OS_EVENT *pevent)
{
    OS_Q      *pq;
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr = 0;
#endif
    uint32_t cpu_sr;


#if 1 //OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return 4; //(OS_ERR_PEVENT_NULL);
    }
    if (pevent->OSEventType != 2/*OS_EVENT_TYPE_Q*/) {     /* Validate event block type                     */
        return 1; //(OS_ERR_EVENT_TYPE);
    }
#endif
    cpu_sr = FAMOS_EnterCriticalSection();
    pq             = (OS_Q *)pevent->OSEventPtr;      /* Point to queue storage structure              */
    pq->OSQIn      = pq->OSQStart;
    pq->OSQOut     = pq->OSQStart;
    pq->OSQEntries = 0;
    FAMOS_LeaveCriticalSection(cpu_sr);
    return 0; //(OS_NO_ERR);
}


/* 23463edc - nearly complete */
void* OSQPend(OS_EVENT *pevent, uint16_t timeout, uint8_t *err)
{
    void      *msg;
    OS_Q      *pq;
	uint32_t cpu_sr;

	if (OSIntNesting != 0)
	{
		*err = 2;
		return 0;
	}

	if (pevent == 0)
	{
		*err = 4;
		return 0;
	}

	if (pevent->OSEventType != 2)
	{
		*err = 1;
		return 0;
	}

	cpu_sr = FAMOS_EnterCriticalSection();

	pq = (OS_Q *)pevent->OSEventPtr;
	if (pq->OSQEntries != 0)
	{
        msg = *pq->OSQOut++;                     /* Yes, extract oldest message from the queue         */
        pq->OSQEntries--;                        /* Update the number of entries in the queue          */
        if (pq->OSQOut == pq->OSQEnd)
        {
            pq->OSQOut = pq->OSQStart;
        }

        FAMOS_LeaveCriticalSection(cpu_sr);
        *err = 0;
        return msg;
	}
	//loc_23463f78
	OSTCBCur->OSTCBStat |= 4; //OS_STAT_Q
	OSTCBCur->OSTCBDly = timeout;

	OS_EventTaskWait(pevent);

	FAMOS_LeaveCriticalSection(cpu_sr);

	OS_Sched();

	cpu_sr = FAMOS_EnterCriticalSection();

	msg = OSTCBCur->OSTCBMsg;
	if (msg != 0)
	{
		OSTCBCur->OSTCBMsg = 0;
		OSTCBCur->OSTCBStat = 0;
		OSTCBCur->OSTCBEventPtr = 0;

		FAMOS_LeaveCriticalSection(cpu_sr);

		*err = 0;
		return msg;
	}
	//loc_23463fdc
	OS_EventTaskRemove(pevent);

	FAMOS_LeaveCriticalSection(cpu_sr);

	*err = 10;
	return 0;
}


/* 23463ffc - complete */
uint8_t OSQPost(OS_EVENT *pevent, void *msg)
{
    OS_Q      *pq;
	uint32_t cpu_sr;

	if (pevent == 0)
	{
		return 4;
	}

	if (msg == 0)
	{
		return 3;
	}

	if (pevent->OSEventType != 2)
	{
		return 1;
	}

	cpu_sr = FAMOS_EnterCriticalSection();

	if (pevent->OSEventGrp != 0)
	{
		OS_EventTaskRdy(pevent, msg, 4);

		FAMOS_LeaveCriticalSection(cpu_sr);

		OS_Sched();
		//->loc_234640bc
	}
	else
	{
		//loc_23464060
	    pq = (OS_Q *)pevent->OSEventPtr;                   /* Point to queue control block                 */
	    if (pq->OSQEntries >= pq->OSQSize)
	    {
			FAMOS_LeaveCriticalSection(cpu_sr);
	        return 30; //(OS_Q_FULL);
	    }
	    //loc_23464084
	    *pq->OSQIn++ = msg;                                /* Insert message into queue                    */
	    pq->OSQEntries++;                                  /* Update the nbr of entries in the queue       */
	    if (pq->OSQIn == pq->OSQEnd)
	    {
	        pq->OSQIn = pq->OSQStart;
	    }
		FAMOS_LeaveCriticalSection(cpu_sr);
	}
	//loc_234640bc
    return 0;
}


/* 23464168 - complete */
void OS_QInit(void)
{
    uint16_t  i;
    OS_Q   *pq1;
    OS_Q   *pq2;

    pq1 = &OSQTbl[0];
    pq2 = &OSQTbl[1];
    for (i = 0; i < (OS_MAX_QS - 1); i++)
    {
        pq1->OSQPtr = pq2;
        pq1++;
        pq2++;
    }
    pq1->OSQPtr = (OS_Q *)0;
    OSQFreeList = &OSQTbl[0];
}



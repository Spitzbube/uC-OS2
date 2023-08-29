
#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

extern uint8_t OSIntNesting; //23492c00
extern uint8_t OSLockNesting; //23492c02 +2 //3a863a
extern uint8_t OSRdyGrp; //23492c05 +5 //bData_3a863d
extern uint8_t OSRunning; //23492c06 +6
extern uint8_t OSTaskCtr; //23492c07 +7 //bData_3a8647
extern OS_EVENT* OSEventFreeList; //23492c0c +0xc
extern RTOS_tTCB* rtos_pTCBFree; //23492c18 +0x18
extern RTOS_tTCB* OSTCBList; //23492c20 +0x20
#if 0
extern RTOS_tTCB* OSTCBCur; //234b2ca8
#endif
extern uint8_t OSRdyTbl[]; //23492c28 +0x28
extern RTOS_tTCB* rtos_arThread[]; //237814dc +64*4 = 237815DC

extern void rtos_init();
extern void rtos_start();
extern void rtos_task_wait(uint16_t);

extern void rtos_sema_enter(OS_EVENT*, int, int*);

extern OS_EVENT* OSMboxCreate(void* pmsg);
extern void* OSMboxPend(OS_EVENT* pevent, int timeout, uint8_t* perr);

extern void OS_QInit(void);
extern OS_EVENT* OSQCreate(void **start, uint16_t size);
extern void* OSQPend(OS_EVENT *pevent, uint16_t timeout, uint8_t *err);
extern uint8_t OSQPost(OS_EVENT *pevent, void *msg);

#endif //KERNEL_H

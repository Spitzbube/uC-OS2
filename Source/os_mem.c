
#include <ucos_ii.h>

OS_MEM           *OSMemFreeList; //23492c30
OS_MEM            OSMemTbl[OS_MAX_MEM_PART]; //235fa20c


/* 234639f4 - complete */
OS_MEM  *OSMemCreate (void *addr, uint32_t nblks, uint32_t blksize, uint8_t *err)
{
    OS_MEM    *pmem;
    uint8_t     *pblk;
    void     **plink;
    uint32_t     i;
    uint32_t  cpu_sr = 0;


#if 0
    if (err == (INT8U *)0) {                          /* Validate 'err'                                */
        return ((OS_MEM *)0);
    }
#endif
    if (addr == (void *)0) {                          /* Must pass a valid address for the memory part.*/
        *err = 0x76; //OS_MEM_INVALID_ADDR;
        return ((OS_MEM *)0);
    }
#if 0
    if (((INT32U)addr & (sizeof(void *) - 1)) != 0){  /* Must be pointer size aligned                  */
        *err = OS_MEM_INVALID_ADDR;
        return ((OS_MEM *)0);
    }
#endif
    if (nblks < 2) {                                  /* Must have at least 2 blocks per partition     */
        *err = 0x6f; //OS_MEM_INVALID_BLKS;
        return ((OS_MEM *)0);
    }
    if (blksize < sizeof(void *)) {                   /* Must contain space for at least a pointer     */
        *err = 0x70; //OS_MEM_INVALID_SIZE;
        return ((OS_MEM *)0);
    }
#if 0
    if ((blksize % sizeof(void *)) != 0) {            /* Must contain space for an integral number ... */
        *err = OS_MEM_INVALID_SIZE;                   /* ... of pointer sized items                    */
        return ((OS_MEM *)0);
    }
#endif

    cpu_sr = FAMOS_EnterCriticalSection();
    pmem = OSMemFreeList;                             /* Get next free memory partition                */
    if (OSMemFreeList != (OS_MEM *)0)
    {               /* See if pool of free partitions was empty      */
        OSMemFreeList = (OS_MEM *)OSMemFreeList->OSMemFreeList;
    }
    FAMOS_LeaveCriticalSection(cpu_sr);
    if (pmem == (OS_MEM *)0)
    {                        /* See if we have a memory partition             */
        *err = 0x6e; //OS_MEM_INVALID_PART;
        return ((OS_MEM *)0);
    }
    plink = (void **)addr;                            /* Create linked list of free memory blocks      */
    pblk  = (uint8_t *)((uint32_t)addr + blksize);
    for (i = 0; i < (nblks - 1); i++)
    {
       *plink = (void *)pblk;                         /* Save pointer to NEXT block in CURRENT block   */
        plink = (void **)pblk;                        /* Position to  NEXT      block                  */
        pblk  = (uint8_t *)((uint32_t)pblk + blksize);    /* Point to the FOLLOWING block                  */
    }
    *plink              = (void *)0;                  /* Last memory block points to NULL              */
    pmem->OSMemAddr     = addr;                       /* Store start address of memory partition       */
    pmem->OSMemFreeList = addr;                       /* Initialize pointer to pool of free blocks     */
    pmem->OSMemNFree    = nblks;                      /* Store number of free blocks in MCB            */
    pmem->OSMemNBlks    = nblks;
    pmem->OSMemBlkSize  = blksize;                    /* Store block size of each memory blocks        */
    *err                = 0; //OS_NO_ERR;
    return (pmem);
}


/* 23463aac - complete */
void  *OSMemGet (OS_MEM *pmem, uint8_t *err)
{
    void      *pblk;
    uint32_t  cpu_sr = 0;

#if 0
    if (err == (INT8U *)0) {                          /* Validate 'err'                                */
        return ((void *)0);
    }
#endif
    if (pmem == (OS_MEM *)0)
    {                        /* Must point to a valid memory partition        */
        *err = 0x74; //OS_MEM_INVALID_PMEM;
        return ((void *)0);
    }

    cpu_sr = FAMOS_EnterCriticalSection();

    if (pmem->OSMemNFree > 0)
    {                       /* See if there are any free memory blocks       */
        pblk                = pmem->OSMemFreeList;    /* Yes, point to next free memory block          */
        pmem->OSMemFreeList = *(void **)pblk;         /*      Adjust pointer to new free list          */
        pmem->OSMemNFree--;                           /*      One less memory block in this partition  */
        FAMOS_LeaveCriticalSection(cpu_sr);
        *err = 0; //OS_NO_ERR;                             /*      No error                                 */
        return (pblk);                                /*      Return memory block to caller            */
    }

    FAMOS_LeaveCriticalSection(cpu_sr);

    *err = 0x71; //OS_MEM_NO_FREE_BLKS;                       /* No,  Notify caller of empty memory partition  */

    return ((void *)0);                               /*      Return NULL pointer to caller            */
}


/* 23463bd4 - complete */
void  OS_MemInit (void)
{
#if OS_MAX_MEM_PART == 1
    OS_MemClr((INT8U *)&OSMemTbl[0], sizeof(OSMemTbl));   /* Clear the memory partition table          */
    OSMemFreeList               = (OS_MEM *)&OSMemTbl[0]; /* Point to beginning of free list           */
#if OS_MEM_NAME_SIZE > 1
    OSMemFreeList->OSMemName[0] = '?';                    /* Unknown name                              */
    OSMemFreeList->OSMemName[1] = OS_ASCII_NUL;
#endif
#endif

#if OS_MAX_MEM_PART >= 2
    OS_MEM  *pmem;
    uint16_t   i;


//    OS_MemClr((uint8_t *)&OSMemTbl[0], sizeof(OSMemTbl));   /* Clear the memory partition table          */
    pmem = &OSMemTbl[0];                                  /* Point to memory control block (MCB)       */
    for (i = 0; i < (OS_MAX_MEM_PART - 1); i++)
    {         /* Init. list of free memory partitions      */
    	pmem->OSMemAddr = 0;
        pmem->OSMemFreeList = (void *)&OSMemTbl[i+1];     /* Chain list of free partitions             */
        pmem->OSMemNFree = 0;
        pmem->OSMemNBlks = 0;
        pmem->OSMemBlkSize = 0;
#if OS_MEM_NAME_SIZE > 1
        pmem->OSMemName[0]  = '?';                        /* Unknown name                              */
        pmem->OSMemName[1]  = OS_ASCII_NUL;
#endif
        pmem++;
    }
    pmem->OSMemFreeList = (void *)0;                      /* Initialize last node                      */
    pmem->OSMemAddr = 0;
    pmem->OSMemNFree = 0;
    pmem->OSMemNBlks = 0;
    pmem->OSMemBlkSize = 0;
#if OS_MEM_NAME_SIZE > 1
    pmem->OSMemName[0]  = '?';                            /* Unknown name                              */
    pmem->OSMemName[1]  = OS_ASCII_NUL;
#endif

    OSMemFreeList       = &OSMemTbl[0];                   /* Point to beginning of free list           */
#endif
}



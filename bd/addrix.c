
//
// 16-Aug-1998
// demo, add records and reindex
//
// Note: OS/2 using the IBM busmastering ATA driver may result in the OS dropping
//       the priority of this task (reason unknown) giving a slow add record rate
//       (100k records take 24 seconds to add instead of 3 seconds).  This can be
//       worked around by using a time-critical priority level 3 or above, or probably
//       by just putting the I/O (the call to BltAddRecord()) in a thread other than
//       thread 1.  SCSI is not affected.  May or may not affect you or other machines.
//
//       For ADD_RND, a large disk cache is recommended for counts > 30,000 (where
//       a (very small) 2MB disk cache is blown out, making record access much slower)
//
//       Note on 16-bit builds:
//       ---------------------
//       For 16-bit builds, be aware that the RECS_TO_ADD constants are limited to 16-bit
//       by the compiler (and the printf() formats for those are just using %u), so you
//       will have to change more than just the constant if you want a value > 65535.

#include <stddef.h>

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_ADDRIX
 #define RECS_TO_ADD 25000
#endif

static TBLT_RETC TBLT_CALLBACK MyFuncCallBack(TBLT_KH *khPtr, ULONG info);


// ------------------------------------------------------
// Reindex

TBLT_RETC DemoReindex(TBLT_KH *khPtr) {

 TBLT_RETC rc = 0;

 ULONG recNo;
 TBLT_IX4CALLBACKPTR oldFN;

 UCHAR workFN[] = "$DEMO_01.TMP";
 UCHAR keyBuffer[MAX_KEY_LENGTH];

 keyBuffer[khPtr->keyLength] = 0;  // 0T

 BltDeleteFile(workFN,0);

 oldFN = khPtr->xCallBackPtr;
 khPtr->xCallBackPtr = MyFuncCallBack;
 rc = BltFuncIx4Reindex(khPtr, workFN, 100, keyBuffer, &recNo);
 khPtr->xCallBackPtr = oldFN;

 return rc;
}


//------------------------------------
// Callback handler (reindex)

static TBLT_RETC TBLT_CALLBACK MyFuncCallBack(TBLT_KH *khPtr, ULONG info) {

 static UINT lastPct = 0;

 if ((UINT)khPtr->reindexPct != lastPct) {
    lastPct = (UINT)khPtr->reindexPct;
    if (lastPct) {
       printf("Reindex pct=%u \r",lastPct);
    }
    else {
       printf("Reindex pct=done  \n");
    }
 }

 return 0;
 info;
}


#ifdef TEST_MODULE_ADDRIX

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;
 time_t startTime, endTime;

 setbuf(stdout,NULL);   // requires since \r used (may look odd without this)

 printf("sizeof TBLT_KH=%u\n",sizeof(TBLT_KH));     // 2048 bytes w/512-byte node
 printf("sizeof TBLT_DH=%u\n",sizeof(TBLT_DH));    // 224 bytes

#if 0
 // sanity checks on structures (unreachable code warning is expected)
 if (TBLT_KH_BUFFER_OFFSET_IS != OFFSET_TBLT_KH_BUFFER) {
    printf("TBLT_KH structure pack is not correct\n");
    return 1;
 }
 if (TBLT_DH_FD1_OFFSET_IS != OFFSET_TBLT_DH_FD1) {
    printf("TBLT_DH structure pack is not the correct\n");
    return 1;
 }
 if (BLTDT_ALIGN4_OFFSET_IS != OFFSETBLT_DT_ALIGN4) {
    printf("BLTDT structure pack is not the correct\n");
    return 1;
 }
#endif

 printf("addrix.c count=%u\n",RECS_TO_ADD);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       printf("Adding Records...");
       time(&startTime);
       rc = DemoAddRecords(dhPtr, RECS_TO_ADD, ADD_SEQ); // ADD_RND likes a big disk cache for reads
       time(&endTime);
       printf("\rAdd %u records took %u secs, rc=%u\n",RECS_TO_ADD,(endTime - startTime),rc);

       if (rc == 0) {

#ifdef BLT_FLATMM

          ULONG tVar;

          // if flat memory model then can expand memory limits (otherwise already at max, 16-bit limits)
          // max total allocation requested by reindex will be two times MASZ plus record length plus a bit

          printf("|current total workspace=%7u bytes\n",BltGetVar(BLTVAR_MASZRX,0));
          printf("|  current single buffer=%7u \n",BltGetVar(BLTVAR_MASZ,0));

          tVar = 1088 * 1024;             // pass ptr to this since on return has previous value
          BltSetVar(&tVar,BLTVAR_MASZRX); // max total memory, in bytes, reindex is allowed to allocate (-1 is no max)
          tVar = 512 * 1024;
          BltSetVar(&tVar,BLTVAR_MASZ);   // max single allocation request size, in bytes, that will be made
          printf("|new max total workspace=%7u \n",BltGetVar(BLTVAR_MASZRX,0));
          printf("|  new max single buffer=%7u \n",BltGetVar(BLTVAR_MASZ,0));

          khPtr->MASZRX =  352 * 1024;  // if -0- uses BLTVAR_MASZRX value (cannot be more than that)
          khPtr->MASZ = 144 * 1024;     // if -0- uses BLTVAR_MASZ value (cannot be more than that)
          printf("|       wanted workspace=%7u \n",khPtr->MASZRX);
          printf("|   wanted single buffer=%7u \n",khPtr->MASZ);
#endif

          time(&startTime);
          rc = DemoReindex(khPtr);
          time(&endTime);
          printf("Reindex took %u secs, rc=%u\n",(endTime - startTime),rc);
          printf("memory requested by reindex=%7lu bytes\n",khPtr->MMUBR);

       }

       // verify order (these do not expect DESCEND())

       if (rc == 0) {
          printf("Getting all keys/recs forward...");
          rc = DemoGetForward(khPtr, RECS_TO_ADD);
          printf("Get Forward, rc=%u\n",rc);

          printf("Getting all keys/recs reverse...");
          rc = DemoGetReverse(khPtr, RECS_TO_ADD);
          printf("Get Reverse, rc=%u\n",rc);
       }

       rc = BltIx4CloseFile(khPtr);
       printf("Index Close, rc=%u\n",rc);
       rc = BltDataCloseFile(dhPtr);
       printf(" Data Close, rc=%u\n",rc);

       free(dhPtr);
       free(khPtr);
    }
 }

 printf("Done.\n");
 return (int)rc;
}
#endif

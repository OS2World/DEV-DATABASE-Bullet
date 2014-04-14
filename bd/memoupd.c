
//
// 16-Aug-1998
// demo, update memo
//

#include "bullet_3.h"
#include "blt3demo.h"

#ifdef TEST_MODULE_MEMOUPD
 #define MEMOS_TO_ADD 500
#endif


// ------------------------------------------------------
// Update memos

TBLT_RETC DemoUpdateMemos(TBLT_DH *dhPtr, ULONG memosToUpdate, UINT updateMode) {

 static ULONG mode2bytes = 0; // size to shrink memos to in mode 2 (set at mode 1)

 TBLT_RETC rc = 0;
 ULONG recNo, memoNo, orgMemoNo, dataBytes;
 ULONG bytesAlloc = 0;

 UCHAR *memoDataPtr = 0;
 UCHAR tmpBuffer[16];
 DATARECORD dataRec;

 bytesAlloc = dhPtr->memoBlockSize + 16;
 memoDataPtr = malloc(bytesAlloc);
 if (memoDataPtr == 0) return 8;
 memset(memoDataPtr,0,bytesAlloc);

 memset(&dataRec,0,sizeof(dataRec));

 switch (updateMode) {
 case 0:
    strcpy(memoDataPtr,"Memo updated, mode 0 (update still has same block size)");
    dataBytes = strlen(memoDataPtr)+1;  // will fit in one block (def block size is 512 bytes
    break;                              // ...with first block having 8 bytes overhead, so 504 bytes there)
 case 1:

    //dhPtr->LOMS = 1;  // flag to leave old memo space as-is (may be faster if lots of clearing needed)

    memset(memoDataPtr,'A',bytesAlloc);
    strcpy(memoDataPtr,"Memo updated, mode 1 (block size expanded [later shrunk])");
    dataBytes = bytesAlloc;

    mode2bytes = strlen(memoDataPtr)+1;
    break;
 case 2:
    // resize does not use a data buffer; it just shrinks the memo size
    // in this example it shrinks the 2-block memo size from case 1: (above) back to 1 block size used

    dataBytes = mode2bytes;
    if (dataBytes == 0) {
       printf("shrink size was not set (previous call in mode 1 required)\n");
       return 1;
    }

    break;
 default:
    rc = 1;
 }

 for (recNo=1; recNo <= memosToUpdate; recNo++) {
    if (rc) break;

    rc = BltDataGetRecord(dhPtr, recNo, &dataRec);
    if (rc == 0) {

       strncpy(tmpBuffer,dataRec.personalMemoNo,10);
       tmpBuffer[10] = 0;
       memoNo = atol(tmpBuffer);

       if (memoNo) {

          // update memo (memo number may change if expanding memo size)
          // starting at memo offset 0 (first byte)

          orgMemoNo = memoNo;
          if (updateMode != 2) {
             rc = BltMemoUpdate(dhPtr, &memoNo, 0, dataBytes, memoDataPtr);
          }
          else {
             rc = BltMemoUpdate(dhPtr, &memoNo, 0, dataBytes, 0); // shrink so null buffer ptr
          }

          // if new memo number update DBF record

          if (rc == 0 && memoNo != orgMemoNo) {

             sprintf(tmpBuffer,"%10.10u",memoNo);
             strncpy(dataRec.personalMemoNo,tmpBuffer,10);
             rc = BltDataUpdateRecord(dhPtr, recNo, &dataRec);
          }
       }
    }
 }

 return rc;
}


#ifdef TEST_MODULE_MEMOUPD

int main(void) {

 TBLT_RETC rc;
 TBLT_DH *dhPtr = 0;
 TBLT_KH *khPtr = 0;
 time_t startTime, endTime;

 setbuf(stdout,NULL);

 printf("memoupd.c count=%u\n",MEMOS_TO_ADD);
 rc = DemoCreateFiles();
 printf("Create Files, rc=%u\n",rc);
 if (rc == 0) {
    rc = DemoOpenFiles(&dhPtr,&khPtr,DBF_ASMODE,IX4_ASMODE);
    printf(" Open Files, rc=%u\n",rc);

    if (rc == 0) {
       printf("Adding Memos/Records...");
       time(&startTime);
       rc = DemoAddMemos(dhPtr, MEMOS_TO_ADD, 0);
       time(&endTime);
       printf("\rAdd %u memos/records took %u secs, rc=%u\n",MEMOS_TO_ADD,(endTime - startTime),rc);

       // Note: mode=1 is disk intensive, long seeks are likely due to the test

       printf("mode0=updates all memos, no expand\n");
       printf("mode1=updates all memos, w/ expand (updates DBF also)\n");
       printf("mode2=updates all memos, shrinks size\n");
       printf("Updating memos...\n");
       time(&startTime);
       rc = DemoUpdateMemos(dhPtr, MEMOS_TO_ADD, 0);
       time(&endTime);
       printf("mode 0: Update %u memos/records took %u secs, rc=%u\n",MEMOS_TO_ADD,(endTime - startTime),rc);

       if (rc == 0) {
          printf("Updating memos, mode=1...");
          time(&startTime);
          rc = DemoUpdateMemos(dhPtr, MEMOS_TO_ADD, 1);
          time(&endTime);
          printf("\rmode 1: Update %u memos/records took %u secs, rc=%u\n",MEMOS_TO_ADD,(endTime - startTime),rc);
       }
       if (rc == 0) {
          time(&startTime);
          rc = DemoUpdateMemos(dhPtr, MEMOS_TO_ADD, 2);
          time(&endTime);
          printf("mode 2: Update %u memos/records took %u secs, rc=%u\n",MEMOS_TO_ADD,(endTime - startTime),rc);
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
